#ifndef PACKET_ENVIRONMENTAL_SENSOR_TYPE_C_H_
#define PACKET_ENVIRONMENTAL_SENSOR_TYPE_C_H_

#pragma pack(push, 1)
class EnvironmentalSensorTypeC {
    public:
        const uint8_t PACKET_LENGTH = 23;

        uint8_t getIaqAccuracy() {
            return m_sensor.sensorAccuracy & 0b00000011;
        }

        bool setIaqAccuracy(uint8_t iaqAccuracy) {
            if (iaqAccuracy > 3) {
                return false;
            }
            m_sensor.sensorAccuracy &= 0b11111100;
            m_sensor.sensorAccuracy |= iaqAccuracy;
            return true; 
        }

        uint8_t getStaticIaqAccuracy() {
            return (m_sensor.sensorAccuracy >> 2) & 0b00000011;
        }

        bool setStaticIaqAccuracy(uint8_t staticIaqAccuracy) {
            if (staticIaqAccuracy > 3) {
                return false;
            }
            m_sensor.sensorAccuracy &= 0b11110011;
            m_sensor.sensorAccuracy |= staticIaqAccuracy << 2;
            return true; 
        }

        uint8_t getCo2Accuracy() {
            return (m_sensor.sensorAccuracy >> 4) & 0b00000011;
        }

        bool setCo2Accuracy(uint8_t co2Accuracy) {
            if (co2Accuracy > 3) {
                return false;
            }
            m_sensor.sensorAccuracy &= 0b11001111;
            m_sensor.sensorAccuracy |= co2Accuracy << 4;
            return true; 
        }

        float getIaq() {
            return m_sensor.iaq;
        }
        
        bool setIaq(float iaq) {
            if (iaq < 0) {
                return false;
            }
            m_sensor.iaq = iaq;
            return true;
        }

        float getStaticIaq() {
            return m_sensor.staticIaq;
        }
        
        bool setStaticIaq(float staticIaq) {
            if (staticIaq < 0) {
                return false;
            }
            m_sensor.staticIaq = staticIaq;
            return true;
        }

        float getCo2() {
            return m_sensor.co2;
        }
        
        bool setCo2(float co2) {
            if (co2 < 0) {
                return false;
            }
            m_sensor.co2 = co2;
            return true;
        }

        float getTemperature() {
            return (float)m_sensor.temperature / 100;
        }

        bool setTemperature(float temperature) {
            if (temperature < -327.68 || temperature > 327.67) {
                return false;
            }
            m_sensor.temperature = temperature * 100;
            return true;
        }

        float getHumidity() {
            return (float)m_sensor.humidity / 100;
        }

        bool setHumidity(float humidity) {
            if (humidity < 0 || humidity > 100) {
                return false;
            }
            m_sensor.humidity = humidity * 100;
            return true;
        }

        float getPressure() {
            return (float)m_sensor.pressure / 100;
        }

        bool setPressure(float pressure) {
            if (pressure < 300 || pressure > 1100) {
                return false;
            }
            m_sensor.pressure = pressure;
            return true;
        }

        float getVoltage() {
            return (float)m_sensor.voltage / 100;
        }

        bool setVoltage(float voltage) {
            if (voltage < 0 || voltage > 5) {
                return false;
            }
            m_sensor.voltage = voltage * 10000;
            return true;
        }

        uint8_t * getPacket() {
            return (uint8_t*)&m_sensor;
        }

    private:
        struct EnvironmentalSensorTypeCStruct {
            uint8_t version = 0x01;
            uint8_t type = 0x10;
            uint8_t sensorAccuracy = 0;
            float iaq = 0;
            float staticIaq = 0;
            float co2 = 0;
            int16_t temperature = 0;
            uint16_t humidity = 0;
            uint16_t pressure = 0;
            uint16_t voltage = 0;
        };
        EnvironmentalSensorTypeCStruct m_sensor;
};
#pragma pack(pop)

#endif  // PACKET_ENVIRONMENTAL_SENSOR_TYPE_C_H