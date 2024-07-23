#ifndef OCRAPI_H
#define OCRAPI_H
#include <vector>
#ifdef OCR_DLL_EXPORTS
#define OCR_DLL_API __declspec(dllexport)
#else
#define OCR_DLL_API __declspec(dllimport)
#endif

typedef struct OCRResult {
	int ptLT_x, ptLT_y;		//���Ͻǣ�x,y)����
	int ptRT_x, ptRT_y;		//���Ͻǣ�x,y)����
	int ptRB_x, ptRB_y;		//���½ǣ�x,y)����
	int ptLB_x, ptLB_y;		//���½ǣ�x,y)����
	const char* text;		//�������
	float score;			//���Ŷ�
};

class OCR_DLL_API OcrApi {

public:
	
	static OcrApi* createInstance(const char* path);  // �����ļ�·�� ����ʵ������

	virtual ~OcrApi() {}; // ��������������Ϊ�麯��

	virtual int Predict(char* Src_data, int Src_width, int Src_height, int Src_channel, long step) = 0;  //ocr��� 
	//���ؼ����������
	virtual void GetOCRResult(int size, std::vector< OCRResult >& ocrResult) = 0;  //��ȡ�������

};

#endif // !OCRAPI_H
