#include <detection-formats.h>
#include <gtest/gtest.h>

#include <string>

// test data
#include "unittest_data.h" // NOLINT

void checkdata(detectionformats::stationInfo stationobject,
		std::string testinfo) {
	// check station
	if (stationobject.site.station.empty() != true) {
		std::string sitestation = stationobject.site.station;
		std::string expectedstation = std::string(STATION);
		ASSERT_STREQ(sitestation.c_str(),
			expectedstation.c_str())<< testinfo.c_str();
	}

	// check channel
	if (stationobject.site.channel.empty() != true) {
		std::string sitechannel = stationobject.site.channel;
		std::string expectedchannel = std::string(CHANNEL);
		ASSERT_STREQ(sitechannel.c_str(),
			expectedchannel.c_str())<< testinfo.c_str();
	}

	// check network
	if (stationobject.site.network.empty() != true) {
		std::string sitenetwork = stationobject.site.network;
		std::string expectednetwork = std::string(NETWORK);
		ASSERT_STREQ(sitenetwork.c_str(),
			expectednetwork.c_str())<< testinfo.c_str();
	}

	// check location
	if (stationobject.site.location.empty() != true) {
		std::string sitelocation = stationobject.site.location;
		std::string expectedlocation = std::string(LOCATION);
		ASSERT_STREQ(sitelocation.c_str(),
			expectedlocation.c_str())<< testinfo.c_str();
	}

	// check latitude
	if (std::isnan(stationobject.site.latitude) != true) {
		double stationlatitude = stationobject.site.latitude;
		double expectedlatitude = LATITUDE;
		ASSERT_EQ(stationlatitude, expectedlatitude);
	}

	// check longitude
	if (std::isnan(stationobject.site.longitude) != true) {
		double stationlongitude = stationobject.site.longitude;
		double expectedlongitude = LONGITUDE;
		ASSERT_EQ(stationlongitude, expectedlongitude);
	}

	// check elevation
	if (std::isnan(stationobject.site.elevation) != true) {
		double stationelevation = stationobject.site.elevation;
		double expectedelevation = ELEVATION;
		ASSERT_EQ(stationelevation, expectedelevation);
	}

	// check quality
	if (std::isnan(stationobject.quality) != true) {
		double stationquality = stationobject.quality;
		double expectedquality = QUALITY;
		ASSERT_EQ(stationquality, expectedquality)<< testinfo.c_str();
	}

	// check enable
	bool stationenabled = stationobject.enable;
	bool expectedenabled = ENABLE;
	ASSERT_EQ(stationenabled, expectedenabled);

	// check use
	bool stationused = stationobject.use;
	bool expectedused = USE;
	ASSERT_EQ(stationused, expectedused);

	// check useforteleseismic
	bool stationuseforteleseismic = stationobject.useforteleseismic;
	bool expecteduseforteleseismic = USEFORTELESEISM;
	ASSERT_EQ(stationuseforteleseismic, expecteduseforteleseismic);

	// check informationRequestor
	if (stationobject.informationRequestor.isempty() != true) {
		// check agencyid
		if (stationobject.informationRequestor.agencyid.empty() != true) {
			std::string sourceagencyid =
				stationobject.informationRequestor.agencyid;
			std::string expectedagencyid = std::string(AGENCYID);
			ASSERT_STREQ(sourceagencyid.c_str(),
				expectedagencyid.c_str())<< testinfo.c_str();
		}

		// check author
		if (stationobject.informationRequestor.author.empty() != true) {
		std::string sourceauthor = stationobject.informationRequestor.author;
		std::string expectedauthor = std::string(AUTHOR);
		ASSERT_STREQ(sourceauthor.c_str(),
			expectedauthor.c_str())<< testinfo.c_str();
		}
	}
}

// tests to see if pick can successfully
// write json output
TEST(StationInfoTest, WritesJSON) {
	detectionformats::stationInfo stationobject;

	// build stationInfo object
	// site subobject
	stationobject.site.station = std::string(STATION);
	stationobject.site.channel = std::string(CHANNEL);
	stationobject.site.network = std::string(NETWORK);
	stationobject.site.location = std::string(LOCATION);
	stationobject.site.latitude = LATITUDE;
	stationobject.site.longitude = LONGITUDE;
	stationobject.site.elevation = ELEVATION;

	stationobject.quality = QUALITY;
	stationobject.enable = ENABLE;
	stationobject.use = USE;
	stationobject.useforteleseismic = USEFORTELESEISM;

	// source informationRequestor
	stationobject.informationRequestor.agencyid = std::string(AGENCYID);
	stationobject.informationRequestor.author = std::string(AUTHOR);

	// build json string
	rapidjson::Document stationdocument;
	std::string pickjson = detectionformats::ToJSONString(
			stationobject.tojson(stationdocument,
					stationdocument.GetAllocator()));

	// read it back in
	rapidjson::Document stationdocument2;
	detectionformats::stationInfo stationobject2(
			detectionformats::FromJSONString(pickjson, stationdocument2));

	// check data values
	checkdata(stationobject2, "");
}

// tests to see if pick can successfully
// read json output
TEST(StationInfoTest, ReadsJSON) {
	// build pick object
	rapidjson::Document stationdocument;
	detectionformats::stationInfo stationobject(
			detectionformats::FromJSONString(std::string(STATIONSTRING),
					stationdocument));

	// check data values
	checkdata(stationobject, "");
}

// tests to see if pick can successfully
// be constructed
TEST(StationInfoTest, Constructor) {
	// use constructor
	detectionformats::stationInfo stationobject(std::string(STATION),
			std::string(CHANNEL), std::string(NETWORK), std::string(LOCATION),
			LATITUDE, LONGITUDE, ELEVATION, QUALITY, ENABLE, USE, USEFORTELESEISM,
			std::string(AGENCYID), std::string(AUTHOR));

	// check data values
	checkdata(stationobject, "Tested Constructor");

	// use constructor
	detectionformats::stationInfo stationobject2(std::string(STATION),
			std::string(CHANNEL), std::string(NETWORK), std::string(LOCATION),
			LATITUDE, LONGITUDE, ELEVATION, QUALITY, ENABLE, USE, USEFORTELESEISM,
			"", "");

	// check data values
	checkdata(stationobject2, "Tested Constructor (without informationRequestor");

	// use alternate constructor
	detectionformats::stationInfo stationobject_altc(
			detectionformats::site(std::string(STATION), std::string(CHANNEL),
					std::string(NETWORK), std::string(LOCATION), LATITUDE, LONGITUDE,
					ELEVATION),
			QUALITY, ENABLE, USE, USEFORTELESEISM,
			detectionformats::source(std::string(AGENCYID),
					std::string(AUTHOR)));

	// check data values
	checkdata(stationobject_altc, "Tested alternate constructor");

	// json constructor (empty)
    rapidjson::Value emptyvalue(rapidjson::kObjectType);
    detectionformats::stationInfo stationobject3(emptyvalue);

    // check data values
	checkdata(stationobject3, "");
}

// tests to see if pick can successfully
// be copy constructed
TEST(StationInfoTest, CopyConstructor) {
	// use constructor
	detectionformats::stationInfo fromstationobject(std::string(STATION),
			std::string(CHANNEL), std::string(NETWORK), std::string(LOCATION),
			LATITUDE, LONGITUDE, ELEVATION, QUALITY, ENABLE, USE, USEFORTELESEISM,
			std::string(AGENCYID), std::string(AUTHOR));

	detectionformats::stationInfo stationobject(fromstationobject);

	// check data values
	checkdata(stationobject, "");
}

// tests to see if pick can successfully
// validate
TEST(StationInfoTest, Validate) {
	detectionformats::stationInfo stationobject;

	// build pick object
	// site subobject
	stationobject.site.station = std::string(STATION);
	stationobject.site.channel = std::string(CHANNEL);
	stationobject.site.network = std::string(NETWORK);
	stationobject.site.location = std::string(LOCATION);

	stationobject.site.latitude = LATITUDE;
	stationobject.site.longitude = LONGITUDE;
	stationobject.site.elevation = ELEVATION;

	stationobject.quality = QUALITY;
	stationobject.enable = ENABLE;
	stationobject.use = USE;
	stationobject.useforteleseismic = USEFORTELESEISM;

	// successful validation
	bool result = stationobject.isvalid();

	// check return code
	ASSERT_EQ(result, true)
		<< "Tested for successful validation (Without informationRequestor).";

	// source informationRequestor
	stationobject.informationRequestor.agencyid = std::string(AGENCYID);
	stationobject.informationRequestor.author = std::string(AUTHOR);

	result = false;
	result = stationobject.isvalid();

	// check return code
	ASSERT_EQ(result, true)
		<< "Tested for successful validation (With informationRequestor).";

	// build bad stationinfo object
	detectionformats::stationInfo badstationobject;

	result = false;
	try {
		// call validation
		result = badstationobject.isvalid();
	} catch (const std::exception &) {
		// don't care what the exception was
	}

	// check return code
	ASSERT_EQ(result, false)<< "Tested for unsuccessful validation.";

	// build bad stationinfo object
	detectionformats::stationInfo badstationobject2;
	stationobject.site.latitude = -9999;
	stationobject.site.longitude = -9999;

	result = false;
	try {
		// call validation
		result = badstationobject2.isvalid();
	} catch (const std::exception &) {
		// don't care what the exception was
	}

	// check return code
	ASSERT_EQ(result, false)<< "Tested for unsuccessful validation.";
}
