#ifndef OCRAPI_H
#define OCRAPI_H
#include <vector>
#ifdef OCR_DLL_EXPORTS
#define OCR_DLL_API __declspec(dllexport)
#else
#define OCR_DLL_API __declspec(dllimport)
#endif

typedef struct OCRResult {
	int ptLT_x, ptLT_y;		//左上角（x,y)坐标
	int ptRT_x, ptRT_y;		//右上角（x,y)坐标
	int ptRB_x, ptRB_y;		//右下角（x,y)坐标
	int ptLB_x, ptLB_y;		//左下角（x,y)坐标
	const char* text;		//检测内容
	float score;			//置信度
};

class OCR_DLL_API OcrApi {

public:
	
	static OcrApi* createInstance(const char* path);  // 配置文件路径 创建实例对像

	virtual ~OcrApi() {}; // 将析构函数声明为虚函数

	virtual int Predict(char* Src_data, int Src_width, int Src_height, int Src_channel, long step) = 0;  //ocr检测 
	//返回检测结果的数量
	virtual void GetOCRResult(int size, std::vector< OCRResult >& ocrResult) = 0;  //获取检测数据

};

#endif // !OCRAPI_H
