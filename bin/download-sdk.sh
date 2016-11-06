#SDK_VERSION=2.9
#PLATFORM_DIR=

SDK_VERSION=3.7
PLATFORM_DIR=aplite/

SDK_ZIP_NAME=PebbleSDK-$SDK_VERSION.tar.gz
URL=http://assets.getpebble.com.s3-website-us-east-1.amazonaws.com/sdk2/$SDK_ZIP_NAME

#SDK_VERSION=4.5
#SDK_ZIP_NAME=pebble-sdk-$SDK_VERSION-linux64.tar.bz2
#URL=https://s3.amazonaws.com/assets.getpebble.com/pebble-tool/$SDK_ZIP_NAME

CACHE_DIR=cache
BUILD_DIR=build
mkdir -p ${CACHE_DIR}

if [ ! -f ${CACHE_DIR}/${SDK_ZIP_NAME} ]; then
    echo "Downloading $URL"
    wget -c -O ${CACHE_DIR}/${SDK_ZIP_NAME}.part $URL && mv ${CACHE_DIR}/${SDK_ZIP_NAME}.part ${CACHE_DIR}/${SDK_ZIP_NAME}
fi

if [ ! -d ${CACHE_DIR}/PebbleSDK-${SDK_VERSION} ]; then
    echo "Unpacking SDK"
    tar -C ${CACHE_DIR} -zxf ${CACHE_DIR}/${SDK_ZIP_NAME}
fi

mkdir -p ${BUILD_DIR}/include
cp -p ${CACHE_DIR}/PebbleSDK-${SDK_VERSION}/Pebble/${PLATFORM_DIR}include/pebble.h ${BUILD_DIR}/include/
cp -p ${CACHE_DIR}/PebbleSDK-${SDK_VERSION}/Pebble/${PLATFORM_DIR}include/pebble_fonts.h ${BUILD_DIR}/include/
cp -p ${CACHE_DIR}/PebbleSDK-${SDK_VERSION}/Pebble/${PLATFORM_DIR}include/pebble_process_info.h ${BUILD_DIR}/include/
cp -p ${CACHE_DIR}/PebbleSDK-${SDK_VERSION}/Pebble/${PLATFORM_DIR}include/pebble_worker.h ${BUILD_DIR}/include/
touch ${BUILD_DIR}/include/pebble_warn_unsupported_functions.h

mkdir -p ${BUILD_DIR}/src
touch ${BUILD_DIR}/src/resource_ids.auto.h

#mkdir -p tests/lib
#mv PebbleSDK-$SDK_VERSION/Pebble/lib/libpebble.a tests/lib/
