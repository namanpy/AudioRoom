# AudioRoom

This is a personal project which was created to allow users to share Video/Audio/Screen Share in a group.

Backend is developed in C++.

Frontend is developed in React.

## Build

### 1. Backend
To compile backend using cmake
```
#Install dependencies on Ubuntu

#OpenSSL
sudo apt-get install libssl-dev

#Boost Dev
sudo apt-get install libboost-all-dev

#Lib Curl
sudo apt-get install libcurl4-openssl-dev


#Building
#!# Remove WEBRTC_DEBUG define from CMakeLists.txt to compile for Public IP connections.

cmake -G "Unix Makefiles" 
cmake -build .


```
### 2. Frontend

```
#For normal debug run
npm install
npm start

#For building
npm install 
npm run build

```
