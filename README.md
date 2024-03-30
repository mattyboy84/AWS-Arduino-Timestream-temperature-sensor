# Arduino-AWS-IoT-Core

## Arduino

This uses the `Arduino MKR WiFi 1010` to publish Temperature & Humidity sensor data to `AWS Timestream` through `AWS IoT Core` using MQTT

![Diagram](images/diagram.png)

![Sketch](images/sketch.png)

## Setup

1. Find your AWS Account & Region specific `AWS IoT Broker Endpoint` - This can be found here:
<https://console.aws.amazon.com/iot/home#/settings>
![Broker Endpoint Image](images/broker-endpoint.png)

2. create the AWS IoT certificate

```bash
aws iot create-keys-and-certificate --set-as-active
```

The Response will be:

```json
{
  "certificateArn": "arn:aws:iot:{Region}:{AccountId}:cert/2c0f72bf-b230-4c49-9e0e-cbf177926d96",
  "certificateId": "2c0f72bf-b230-4c49-9e0e-cbf177926d96",
  "certificatePem": "-----BEGIN CERTIFICATE-----\n{Certificate}\n-----END CERTIFICATE-----\n",
  "keyPair": {
    "PublicKey": "-----BEGIN PUBLIC KEY-----\n{Public Key Material}\n-----END PUBLIC KEY-----\n",
    "PrivateKey": "-----BEGIN RSA PRIVATE KEY-----\n{Private Key Material}\n-----END RSA PRIVATE KEY-----\n"
  }
}
```

3. Prepare `arduino_secrets.h` file
   1. Enter your Wifi name & password
   2. Enter your Account & Region specific `AWS IoT Broker Endpoint` from Step 1
   3. Enter a unique identifier for the device.
   4. Enter the complete `Device Certificate` & `Device Private Key` from Step 2
![Broker Endpoint Image](images/arduino_secrets.png)

4. deploy the `template.yaml` including the `certificateArn` parameter from step 2. The template will listen on a topic with the same name as the stack.
5. upload the `.ino` file to Arduino using the Arduino IDE
6. The board will now publish the Temperature & Humidity data from the `DHT22` sensor and publish it to Timestream through `AWS IoT Core`


## Arduino Logs

![](images/Arduino_logs.png)

## Timestream Example

![](images/Timestream_example.png)
