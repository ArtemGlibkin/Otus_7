#pragma once
#include <string>
#include <memory>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <boost/algorithm/hex.hpp>
#include "FileMonitorParameters.h"

class Hasher
{
public:
	virtual std::string getDigest(const std::string& in) = 0;
};

class MD5Hasher : public Hasher
{
	using digest = boost::uuids::detail::md5::digest_type;
	std::string hashToString(const digest& md5hash)
	{
		const auto charDigest = reinterpret_cast<const char*>(&md5hash);
		std::string result;

		boost::algorithm::hex(charDigest, charDigest + sizeof(digest), std::back_inserter(result));
		return result;
	}
public:
	std::string getDigest(const std::string& in)
	{
		boost::uuids::detail::md5 md5;
		md5.process_bytes(in.c_str(), in.size());
		digest md5digest;
		md5.get_digest(md5digest);
		return hashToString(md5digest);
	}
};

class SHA1Hasher : public Hasher
{
	using digest = boost::uuids::detail::sha1::digest_type;
	std::string hashToString(const digest& md5hash)
	{
		const auto charDigest = reinterpret_cast<const char*>(&md5hash);
		std::string result;

		boost::algorithm::hex(charDigest, charDigest + sizeof(digest), std::back_inserter(result));
		return result;
	}
public:
	std::string getDigest(const std::string& in)
	{
		boost::uuids::detail::sha1 sha1;
		sha1.process_bytes(in.c_str(), in.size());
		digest sha1digest;
		sha1.get_digest(sha1digest);
		return hashToString(sha1digest);
	}
};

std::unique_ptr<Hasher> getHasher(HashAlgorithms hash)
{
	switch (hash)
	{
		case HashAlgorithms::MD5:
			return std::make_unique<MD5Hasher>();
		case HashAlgorithms::SHA1:
			return std::make_unique<SHA1Hasher>();
	}
	return std::make_unique<MD5Hasher>();
};