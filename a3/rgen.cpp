#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

//obtain random number from /dev/random
int rand_num_gen(int min_num, int max_num) { // refer to "Advanced linux programming" - Listing 6.1
	unsigned int num = 42;

	int fd = -1;

	if ((fd = open("/dev/urandom", O_RDONLY)) == -1) {

		std::cout << "unable to open /dev/urandom." << std::endl;

		exit(1);

	}
	read(fd, &num, sizeof(int));
	close(fd);
	return  (min_num + (num % (max_num - min_num + 1)));
}

std::string rand_strt_name_gen(int len) {
	std::string str_random = "abcdefghijklmnopqrstuvwxyz"; //for random streetname 
	std::string street_name;
	int arg;
	//creating random streetnames for the length of 4
	while (street_name.size() != len) {
		arg = ((rand() % (str_random.size() - 1)));
		// std::cout<<"debug37 "<<rand<<std::endl;
		street_name += str_random.substr(arg, 1);
		// std::cout<<"debug39 "<<street_name<<std::endl;
	}
	return street_name;
}


// Reference from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
bool lineSegmentIntersection(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{

	int Minxx01 = std::min(x0, x1);
	int Minyy01 = std::min(y0, y1);
	int Minxx23 = std::min(x2, x3);
	int Minyy23 = std::min(y2, y3);
	int Maxxx01 = std::max(x0, x1);
	int Maxyy01 = std::max(y0, y1);
	int Maxxx23 = std::max(x2, x3);
	int Maxyy23 = std::max(y2, y3);

	if (x1 != x0 && x2 != x3)
	{
		float k1 = (y1 - y0) / (x1 - x0);
		float k2 = (y3 - y2) / (x3 - x2);
		if (k1 == k2)
		{ //??d = abs(c1-c2) / sqrt(a*a+b*b)
			float d1 = std::abs(y0 * (x1 - x0) - x0 * (y1 - y0) - y2 * (x3 - x2) + x2 * (y3 - y2)); 
			if (d1 == 0)
			{
				if ((x2>Minxx01 && x2<Maxxx01 && y2>Minyy01 && y2<Maxyy01) || (x3>Minxx01 && x3<Maxxx01 && y3>Minyy01 && y3<Maxyy01)
					|| (x0>Minxx23 && x0<Maxxx23 && y0>Minyy23 && y0<Maxyy23) || (x1>Minxx23 && x1<Maxxx23 && y1>Minyy23 && y1<Maxyy23))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else {
			return false;
		}
	}
}

//checking the point is overlap
bool point_overlap(std::vector <int> points_lst,std::vector< std::vector<int> > points_vect ) {
	for (int i = 2; i < points_lst.size(); i = i + 2) {
		for (int j = 0; j < points_vect.size(); j + 2) {
			for (int k = 2; k < points_vect[j].size(); k + 2) {
				if (lineSegmentIntersection(points_lst[i], points_lst[i + 1],points_lst[i-2],points_lst[i-1],points_vect[j][k],points_vect[j][k+1],points_vect[j][k-2],points_vect[j][k-1] )) {
					return true;
				}
			}
		}
	}
	return false;
}
//checking for same points
bool check_duplicate_point(int x1, int y1, int x2, int y2) {

	if (x1 == x2 && y1 == y2) {
		return true;
	}
	else {
		return false;
	}
}
//checking the point is valid
bool check_valid_point(std::vector <int> points_lst, int a, int b) {
	int len = points_lst.size();
	if (len == 0) {
		return true;
	}
	else if (len == 2) {
		if ((check_duplicate_point(a, b, points_lst[0], points_lst[1]))) {
			return false;
		}
		return true;
	}
	else {
		for (int i = 0; i < points_lst.size(); i = i + 2) {
			if (check_duplicate_point(a, b, points_lst[i], points_lst[i + 1])) {
				return false;
			}
		}
	}
}
//Reference from https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/

int main(int argc, char **argv) {

	char ch;
	int wait_time_k = 5, num_range_k = 20, strt_k = 10, line_seg_k = 5;
	while ((ch = getopt(argc, argv, "s:n:l:c")) != -1) {

		switch (ch)
		{
		//streets value should be [2, k].default k = 10
		case 's':
				strt_k = std::atoi(optarg);
				if(strt_k<2){
					std::cerr<<"Error: S value should not be less than 2"<<std::endl;
					return 1;
				}
				break;
		//Line segments value should be in[1, k].Default: k = 5.
		case 'n':
				line_seg_k = std::atoi(optarg);
				if(line_seg_k<1){
					std::cerr<<"Error: n value should not be less than 1"<<std::endl;
					return 1;
				}
				break;
		// wait value should be [5, k] .Default: k = 5.
		case 'l':
				wait_time_k = std::atoi(optarg);
				if(wait_time_k<5){
					std::cerr<<"Error: l value should not be less than 5"<<std::endl;
					return 1;
				}
				break;
		// (x, y) coordinates range [-k, k]. Default:k = 20.
		case 'c':
				num_range_k = std::atoi(optarg);
				if(num_range_k<1){
                    std::cerr<<"Error: c value should not be less than 1"<<std::endl;
                    return 1;
                }
				break;
		case '?':
				std::cerr<<"Error: Flags are not valid"<<std::endl;
				return 1;
		default:
				std::cerr<<"Error: provide valid flag"<<std::endl;
				return 1;
		}
	}
	int street_num, line_num, wait_num; //point_num;
	int cnt = 0;
	std::string street_name;
	std::vector <std::string> street_list;

	std::vector< std::vector<int> > points_vect;
	while (1) {
		street_num = rand_num_gen(2, strt_k);
		wait_num = rand_num_gen(5, wait_time_k);
		// point_num = rand_num_gen(20, num_range_k);


		while (cnt < street_num) {
			std::vector <int> points_lst; //x_str, y_str;
			line_num = rand_num_gen(1, line_seg_k);

			int count = 0;
			for (int l = 0; l <= 2 * line_num; l++) {
				int x_value, y_value;

				x_value = rand_num_gen(-num_range_k, num_range_k);
				y_value = rand_num_gen(-num_range_k, num_range_k);

				if (check_valid_point(points_lst, x_value, y_value) && !point_overlap(points_lst,points_vect))
				{
					points_lst.push_back(x_value);
					points_lst.push_back(y_value);

					count = 0;
				}
				else
				{
					if (count > 25)
					{
						std::cerr << "Error: Reached the 25 valid input simultaneous attempts" << std::endl;
						std::exit(1);
					}
					count++;
					continue;
				}
				l--;
			}
			street_name = rand_strt_name_gen(4);
			street_list.push_back(street_name);

			points_vect.push_back(points_lst);

			std::cout << "add " << "\"" << street_name << "\" ";
			for (int i = 0; i <= line_num; i++) {
				std::cout << "(" << points_lst[i] << "," << points_lst[i + 1] << ") ";
			}
			std::cout << std::endl;
			cnt++;
		}
		std::cout << "gg" << std::endl;

		sleep(wait_num);

		for (int i = 0; i < street_list.size(); i++) {
			std::cout << "rm " << "\"" << street_list[i] << "\"" << std::endl;
		}
		street_list.clear();
		cnt = 0;

	}

	return 0;
}