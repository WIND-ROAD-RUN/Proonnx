#include "ocrwork.h"
#include <iostream>
#include"ocrPutText.h"


// 使用一个结构体来表示单词和其对应的列信息
struct WordInfo {
    std::vector<std::string> words;
    std::vector<std::vector<int>> columns;
    std::vector<std::string> states;
};



ocrwork::ocrwork()
{

}


const cv::Scalar colorWaterBlue(230, 255, 102);
const cv::Scalar colorBlue(255, 0, 0);
const cv::Scalar colorYellow(0, 255, 255);
const cv::Scalar colorRed(0, 0, 255);
const cv::Scalar colorBlack(0, 0, 0);
const cv::Scalar colorGray(200, 200, 200);
const cv::Scalar colorSystem(240, 240, 240);
const cv::Scalar colorGreen(0, 255, 0);
const cv::Scalar colorWhite(255, 255, 255);
const cv::Scalar colorPurple(214, 112, 218);
const cv::Scalar colorGoldenrod(15, 185, 255);









void  ocrwork::drawView(cv::Mat& m_matSrc, std::vector<OCRResult>ocr_result)
{

	for (int i = 0; i < ocr_result.size(); i++)
    {
    cv::Point ptLT(ocr_result[i].ptLT_x, ocr_result[i].ptLT_y);
    cv::Point ptLB(ocr_result[i].ptLB_x, ocr_result[i].ptLB_y);
    cv::Point ptRB(ocr_result[i].ptRB_x, ocr_result[i].ptRB_y);
    cv::Point ptRT(ocr_result[i].ptRT_x, ocr_result[i].ptRT_y);

    cv::line(m_matSrc, ptLT, ptRT, colorRed, 1);
    cv::line(m_matSrc, ptLT, ptLB, colorGreen, 1);
    cv::line(m_matSrc, ptLB, ptRB, colorRed, 1);
    cv::line(m_matSrc, ptRB, ptRT, colorGreen, 1);
    std::cout << "text: " << ocr_result[i].text << std::endl;

		putText::putTextZH(m_matSrc, ocr_result[i].text, cv::Point(ptLT.x, ptLT.y - 20), cv::Scalar(0, 255, 0), 30, "微软雅黑");
    }


}
void ocrwork::initial()
{
    const char* path = "config/gflags.conf";
    // OcrApi::LoadFlagfile(path);

    qDebug() << "path";
    qDebug() << path;

    ocrApi = OcrApi::createInstance(path);


}

void ocrwork::testOcr(cv::Mat srcMat, std::vector<OCRResult>& ocrResult)
{
   


    int size = ocrApi->Predict((char*)srcMat.data, srcMat.cols, srcMat.rows, srcMat.channels(), srcMat.step);


    ocrApi->GetOCRResult(size, ocrResult);


  



    

    


}
