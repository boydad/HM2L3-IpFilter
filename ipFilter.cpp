#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

std::vector<std::string> split(const std::string &str, char d) {
	std::vector<std::string> r;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of(d);
	while (stop != std::string::npos) {
		r.push_back(str.substr(start, stop - start));

		start = stop + 1;
		stop = str.find_first_of(d, start);
	}

	r.push_back(str.substr(start));

	return r;
}

using Ip = std::vector<std::string>;
using IpPool = std::vector<Ip>;

IpPool loadIpPool(std::istream& is){
	IpPool ipPool;
	for (std::string line; std::getline(is, line);) {
		std::vector<std::string> v = split(line, '\t');
		ipPool.push_back(split(v.at(0), '.'));
	}
	return std::move(ipPool);
}

auto cmpIp = [](const std::vector<std::string>& ip1, const std::vector<std::string>& ip2) -> bool {
	for (int i = 0; i < 4; ++i) {
		if (std::stoi(ip1[i]) > std::stoi(ip2[i]))
			return true;
		if (std::stoi(ip1[i]) < std::stoi(ip2[i]))
			return false;
	}
	return true;
};

std::ostream& operator<<(std::ostream& os, const Ip& ip){
	for (Ip::const_iterator ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part) {
		if (ip_part != ip.cbegin())
			std::cout << ".";
		std::cout << *ip_part;
	}
	return os;
}

template< typename L>
IpPool filter(const IpPool& ipPool, L condition){
	IpPool ipFilt;
	for(IpPool::const_iterator ip = ipPool.cbegin(); ip != ipPool.cend(); ++ip)
		if (condition(*ip))
			ipFilt.push_back(*ip);
	return std::move(ipFilt);
}

auto fByOne = [](const Ip& ip)->bool{
														return ip.at(0) == std::string("1");
													};

auto fByTwo = [](const Ip& ip)->bool{
														return ip.at(0) == std::string("46") and 
																	ip.at(1) == std::string("70");
													};		
													
auto fByAny = [](const Ip& ip)->bool{
														for(int i=0; i<4; ++i)
															if(ip.at(i) == std::string("46"))
																return true;
														return false;
													};													
													

int main(int argc, char const *argv[]) {
	try {
		IpPool ipPool = loadIpPool(std::cin);

		std::sort(ipPool.begin(), ipPool.end(), cmpIp);
		for (auto&& ip : ipPool)
			std::cout<< ip << std::endl;

		{
			IpPool Pool = filter(ipPool, fByOne);			
			for (auto&& ip: Pool)
					std::cout<< ip << std::endl;
		}

		{
			IpPool Pool = filter(ipPool,fByTwo);			
			for (auto&& ip: Pool)
					std::cout<< ip << std::endl;
		}

		{
			IpPool Pool = filter(ipPool, fByAny);			
			for (auto&& ip: Pool)
					std::cout<< ip << std::endl;
		}
		
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
