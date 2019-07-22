#include <stationInfo.h>

#include <string>
#include <limits>
#include <vector>

// JSON Keys
#define SITE_KEY "Site"
#define QUALITY_KEY "Quality"
#define ENABLE_KEY "Enable"
#define USE_KEY "Use"
#define USEFORTELESEISMIC_KEY "UseForTeleseismic"
#define INFORMATIONREQUESTOR_KEY "InformationRequestor"

namespace detectionformats {
stationInfo::stationInfo() {
	type = STATIONINFO_TYPE;
	site = detectionformats::site();
	quality = std::numeric_limits<double>::quiet_NaN();
	enable = true;
	use = true;
	useforteleseismic = false;
	informationRequestor = detectionformats::source();
}

stationInfo::stationInfo(std::string newstation, std::string newchannel,
							std::string newnetwork, std::string newlocation,
							double newlatitude, double newlongitude,
							double newelevation, double newquality,
							bool newenable, bool newuse,
							bool newuseforteleseismic, std::string newagencyid,
							std::string newauthor) {
	type = STATIONINFO_TYPE;
	site = detectionformats::site(newstation, newchannel, newnetwork,
									newlocation, newlatitude, newlongitude, newelevation);
	quality = newquality;
	enable = newenable;
	use = newuse;
	useforteleseismic = newuseforteleseismic;
	informationRequestor = detectionformats::source(newagencyid, newauthor);
}

stationInfo::stationInfo(detectionformats::site newsite,
							double newquality, bool newenable, bool newuse,
							bool newuseforteleseismic,
							detectionformats::source newinformationrequestor) {
	type = STATIONINFO_TYPE;
	stationInfo::site = newsite;
	quality = newquality;
	enable = newenable;
	use = newuse;
	useforteleseismic = newuseforteleseismic;
	informationRequestor = newinformationrequestor;
}

stationInfo::stationInfo(rapidjson::Value &json) {
	// required values
	// type
	if ((json.HasMember(TYPE_KEY) == true)
			&& (json[TYPE_KEY].IsString() == true))
		type = std::string(json[TYPE_KEY].GetString(),
							json[TYPE_KEY].GetStringLength());
	else
		type = "";

	// site
	if ((json.HasMember(SITE_KEY) == true)
			&& (json[SITE_KEY].IsObject() == true)) {
		rapidjson::Value & sitevalue = json["Site"];
		site = detectionformats::site(sitevalue);
	} else {
		site = detectionformats::site();
	}

	// optional values
	// quality
	if ((json.HasMember(QUALITY_KEY) == true)
			&& (json[QUALITY_KEY].IsNumber() == true)
			&& (json[QUALITY_KEY].IsDouble() == true)) {
		quality = json[QUALITY_KEY].GetDouble();
	} else {
		quality = std::numeric_limits<double>::quiet_NaN();
	}

	// enable
	if ((json.HasMember(ENABLE_KEY) == true)
			&& (json[ENABLE_KEY].IsBool() == true)) {
		enable = json[ENABLE_KEY].GetBool();
	} else {
		enable = true;
	}

	// use
	if ((json.HasMember(USE_KEY) == true)
			&& (json[USE_KEY].IsBool() == true)) {
		use = json[USE_KEY].GetBool();
	} else {
		use = true;
	}

	// useforteleseismic
	if ((json.HasMember(USEFORTELESEISMIC_KEY) == true)
			&& (json[USEFORTELESEISMIC_KEY].IsBool() == true)) {
		useforteleseismic = json[USEFORTELESEISMIC_KEY].GetBool();
	} else {
		useforteleseismic = false;
	}

	// informationRequestor
	if ((json.HasMember(INFORMATIONREQUESTOR_KEY) == true)
			&& (json[INFORMATIONREQUESTOR_KEY].IsObject() == true)) {
		rapidjson::Value & sourcevalue = json[INFORMATIONREQUESTOR_KEY];
		informationRequestor = detectionformats::source(sourcevalue);
	} else {
		informationRequestor = detectionformats::source();
	}
}

stationInfo::stationInfo(const stationInfo &newstation) {
	type = STATIONINFO_TYPE;
	site = newstation.site;
	quality = newstation.quality;
	enable = newstation.enable;
	use = newstation.use;
	useforteleseismic = newstation.useforteleseismic;
	informationRequestor = newstation.informationRequestor;
}

stationInfo::~stationInfo() {
}

rapidjson::Value & stationInfo::tojson(
		rapidjson::Value &json,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
	json.SetObject();

	// required values
	// type
	rapidjson::Value typevalue;
	typevalue.SetString(rapidjson::StringRef(type.c_str()), allocator);
	json.AddMember(TYPE_KEY, typevalue, allocator);

	// site
	rapidjson::Value sitevalue(rapidjson::kObjectType);
	site.tojson(sitevalue, allocator);
	json.AddMember(SITE_KEY, sitevalue, allocator);

	// optional values
	// quality
	if (std::isnan(quality) != true) {
		json.AddMember(QUALITY_KEY, quality, allocator);
	}

	// enable
	json.AddMember(ENABLE_KEY, enable, allocator);

	// use
	json.AddMember(USE_KEY, use, allocator);

	// useforteleseismic
	json.AddMember(USEFORTELESEISMIC_KEY, useforteleseismic, allocator);

	// informationRequestor
	if (informationRequestor.isempty() != true) {
		rapidjson::Value sourcevalue(rapidjson::kObjectType);
		informationRequestor.tojson(sourcevalue, allocator);
		json.AddMember(INFORMATIONREQUESTOR_KEY, sourcevalue, allocator);
	}
	return (json);
}

std::vector<std::string> stationInfo::geterrors() {
	std::vector<std::string> errorlist;

	// check for required data
	// Type
	if (type != STATIONINFO_TYPE) {
		// wrong type
		errorlist.push_back("Non-stationInfo type in stationInfo class.");
	}

	// site
	if (site.isvalid() != true) {
		// site not found
		errorlist.push_back(
				"Site object did not validate in stationInfo class.");
	}

	// latitude
	if (std::isnan(site.latitude) == true) {
		// latitude not found
		errorlist.push_back("No Latitude in stationInfo class.");
	} else if ((site.latitude < -90) || (site.latitude > 90)) {
		errorlist.push_back("Latitude in stationInfo class not in the range of -90 "
				"to 90.");
	}

	// longitude
	if (std::isnan(site.longitude) == true) {
		// longitude not found
		errorlist.push_back("No Longitude in stationInfo class.");
	} else if ((site.longitude < -180) || (site.longitude > 180)) {
		errorlist.push_back("Longitude in stationInfo class not in the range of -180 "
				"to 180.");
	}

	// elevation
	if (std::isnan(site.elevation) == true) {
		// elevation not found
		errorlist.push_back("No Elevation in stationInfo class.");
	} else if ((site.elevation < -500) || (site.elevation > 8900)) {
			errorlist.push_back("Elevation in stationInfo class not in the range of "
				"-500 to 8900.");
	}

	// optional data
	// Currently no validation criteria for optional values Quality,
	// Enable, Use, and UseForTeleseismic.

	// informationRequestor
	if (informationRequestor.isempty() != true) {
		if (informationRequestor.isvalid() != true) {
			// site not found
			errorlist.push_back(
					"InformationRequestor object did not validate in stationInfo class.");
		}
	}

	// return the list of errors
	return (errorlist);
}
}  // namespace detectionformats
