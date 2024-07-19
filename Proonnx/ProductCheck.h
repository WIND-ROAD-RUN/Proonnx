#ifndef PRODUCTCHECK_H_
#define PRODUCTCHECK_H_

#include<queue>
#include<functional>

struct ProductCheckUtilty
{
    enum ProductCheckInfo
    {
        PASS = 0,
        EXCEEDING_THRESHOLD = 1,
        WITHIN_THRESHOLD = 2
    };

    template<typename T>

    using RWqueue = std::queue<T>;

};

template<typename productClass,typename standardValue>
class ProductCheck
{
    using ProductCheckInfo = ProductCheckUtilty::ProductCheckInfo;

    template<typename T>
    using RWqueue = ProductCheckUtilty::RWqueue<T>;

    using Compare = std::function<bool(productClass, standardValue)>;

private:
    unsigned int m_kConsecutiveErrorThreshold;

    bool m_lastProductCheck{ false};

    unsigned m_count{0};

private:
    RWqueue<productClass> m_productHitoryQueue;

    Compare m_compare;

private:
    void push_product(productClass product);

    productClass pop_product();

public:
    explicit ProductCheck
    (int kConsecutiveErrorThreshold, Compare compareFunction)
    :m_kConsecutiveErrorThreshold(kConsecutiveErrorThreshold),m_compare(compareFunction){}

public:
    ProductCheckInfo check(productClass product, standardValue standard);

    const RWqueue<productClass> &
        getHistoryProductList() const
    { return m_productHitoryQueue; }
};

template <typename productClass, typename standardValue>
void
ProductCheck<productClass,standardValue>::push_product
(productClass product)
{
    m_productHitoryQueue.push(product);
}

template <typename productClass, typename standardValue>
productClass
ProductCheck<productClass, standardValue>::pop_product
()
{
    if (m_productHitoryQueue.size() > m_kConsecutiveErrorThreshold + 1)
    {
        auto result = m_productHitoryQueue.front();
        m_productHitoryQueue.pop();
        return result;
    }
    else
    {
        return productClass();
    }
}

template <typename productClass, typename standardValue>
typename ProductCheck<productClass, standardValue>::ProductCheckInfo
ProductCheck<productClass, standardValue>::check
(productClass product, standardValue standard)
{
    push_product(product);
    //printInfo();
    auto compareResult = m_compare(product, standard);
    //qDebug()<< "bool"<< compareResult;
    if (compareResult == false && m_lastProductCheck == false)
    {
        m_count++;
        //qDebug() << "after count ++ is:" << m_count;
    }
    else
    {
        m_count = 0;
        //qDebug() << "count==0" ;
    }
    m_lastProductCheck = compareResult;
    pop_product();
    if (m_count >= m_kConsecutiveErrorThreshold)
    {
        //qDebug() << "OUT";
        //qDebug() << "";
        return ProductCheckInfo::EXCEEDING_THRESHOLD;
    }
    else
    {
        //qDebug() << "PASS";
        //qDebug() << "";
        return ProductCheckInfo::WITHIN_THRESHOLD;
    }
}


#endif
