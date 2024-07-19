#ifndef PRODUCTCONFIGLOADER_H_
#define PRODUCTCONFIGLOADER_H_

#include<string>

struct ProductConfig {
	std::string productName;
	long RecognitionRange;
	int gain;
	int ptLT_x, ptLT_y;		
	int ptRT_x, ptRT_y;		
	int ptRB_x, ptRB_y;		
	int ptLB_x, ptLB_y;		
};

class ProductConfigLoader {
public:
	ProductConfig loadConfig(const std::string & configName);
	ProductConfig loadConfig(int id);
	bool storeConfig(const std::string& ProductConfig);

};

#endif // !PRODUCTCONFIGLOADER_H_
