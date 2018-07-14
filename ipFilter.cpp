#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>



std::vector<std::string> split(const std::string &str, char d) {
// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
	std::vector<std::string> r;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of(d);
	while (stop != std::string::npos) {
		r.emplace_back(str.substr(start, stop - start));

		start = stop + 1;
		stop = str.find_first_of(d, start);
	}

	r.emplace_back(str.substr(start));

	return r;
}

using Ip = std::vector<int>;
using IpPool = std::vector<Ip>;

IpPool loadIpPool(std::istream& is){
	try{
		IpPool ipPool;
		for (std::string line; std::getline(is, line);) {
			std::vector<std::string> v = split(line, '\t');
			Ip ip;
			ip.reserve(4);
			for(auto&& ipPart: split(v.at(0), '.'))
				ip.emplace_back(stoi(ipPart));
			ipPool.emplace_back(ip);
		}
		return ipPool;
	}catch (const std::exception &e) {
		std::cerr << "Error in loading ip pool!\n:" << e.what() << std::endl;
		throw e;
	}
}

std::ostream& operator<<(std::ostream& os, const Ip& ip){
	for (Ip::const_iterator ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part) {
		if (ip_part != ip.cbegin())
			std::cout << ".";
		std::cout << *ip_part;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const IpPool& pool){
	for (auto&& ip: pool)
		os<< ip << std::endl;
	return os;
}

template< typename L>
IpPool filter(const IpPool& ipPool, L condition){
	IpPool ipFilt;
	for(auto&& ip : ipPool)
		if (condition(ip))
			ipFilt.emplace_back(ip);
	return ipFilt;
}

int main(int argc, char const *argv[]) {
	try {
		IpPool ipPool = loadIpPool(std::cin);
		
		auto cmpIp = [](const Ip& ip1, const Ip& ip2) -> bool {
										for (int i = 0; i < 4; ++i) {
											if(ip1[i] == ip2[i])
												continue;
											return ip1[i] > ip2[i];
										}
										return true;
									};									
		std::sort(ipPool.begin(), ipPool.end(), cmpIp);
		std::cout<< ipPool;

		auto fByOne = [](const Ip& ip)->bool{
													return ip.at(0) == 1;};
		std::cout<< filter(ipPool, fByOne);			


		auto fByTwo = [](const Ip& ip)->bool{
												return ip.at(0) == 46  
															 and ip.at(1) == 70;};											
		std::cout<< filter(ipPool,fByTwo);			


		auto fByAny = [](const Ip& ip)->bool{
										for(int i=0; i<4; ++i)
											if(ip.at(i) == 46)
												return true;
										return false;
									};
		std::cout<< filter(ipPool, fByAny);			

		
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
