#SDK_VERSION=2.9
#PLATFORM_DIR=

SDK_VERSION=3.7
PLATFORM_DIR=aplite/

SDK_ZIP_NAME=PebbleSDK-$SDK_VERSION.tar.gz
URL=http://assets.getpebble.com.s3-website-us-east-1.amazonaws.com/sdk2/$SDK_ZIP_NAME

#SDK_VERSION=4.5
#SDK_ZIP_NAME=pebble-sdk-$SDK_VERSION-linux64.tar.bz2
#URL=https://s3.amazonaws.com/assets.getpebble.com/pebble-tool/$SDK_ZIP_NAME

BUILD_DIR=build
mkdir -p ${BUILD_DIR}

echo "Downloading $URL"
wget -c -O ${BUILD_DIR}/${SDK_ZIP_NAME} $URL

echo "Unpacking SDK"
tar -C ${BUILD_DIR} -zxf ${BUILD_DIR}/${SDK_ZIP_NAME}

mkdir -p build/include
cp -p build/PebbleSDK-${SDK_VERSION}/Pebble/${PLATFORM_DIR}include/pebble.h build/include/
cp -p build/PebbleSDK-${SDK_VERSION}/Pebble/${PLATFORM_DIR}include/pebble_fonts.h build/include/
cp -p build/PebbleSDK-${SDK_VERSION}/Pebble/${PLATFORM_DIR}include/pebble_process_info.h build/include/
cp -p build/PebbleSDK-${SDK_VERSION}/Pebble/${PLATFORM_DIR}include/pebble_worker.h build/include/
touch build/include/pebble_warn_unsupported_functions.h

mkdir -p build/src
touch build/src/resource_ids.auto.h

#mkdir -p tests/lib
#mv PebbleSDK-$SDK_VERSION/Pebble/lib/libpebble.a tests/lib/
