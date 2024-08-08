#ifndef OCRAPI_H
#define OCRAPI_H
#include <vector>
#ifdef OCR_DLL_EXPORTS
#define OCR_DLL_API __declspec(dllexport)
#else
#define OCR_DLL_API __declspec(dllimport)
#endif

typedef struct OCRResult {
	int ptLT_x, ptLT_y;		//Upper left corner (x, y) coordinates
	int ptRT_x, ptRT_y;		//Upper right corner (x, y) coordinates
	int ptRB_x, ptRB_y;		//Bottom right corner (x, y) coordinates
	int ptLB_x, ptLB_y;		//Bottom left corner (x, y) coordinates
	const char* text;		//Testing content
	float score;			//Confidence level
};

class OCR_DLL_API OcrApi {

public:
	
	static OcrApi* createInstance(const char* path);  // Create instance object by configuring file path

	virtual ~OcrApi() {}; // Declare destructor as a virtual function

	virtual int Predict(char* Src_data, int Src_width, int Src_height, int Src_channel, long step) = 0;  //OCR detection
//Return the number of test results
	virtual void GetOCRResult(int size, std::vector< OCRResult >& ocrResult) = 0;  //Obtain detection data

};

#endif // !OCRAPI_H
