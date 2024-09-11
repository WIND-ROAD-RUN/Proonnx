#include <QApplication>
#include <QTreeView>
#include <QStandardItemModel>
#include<QHeaderView>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 创建 QTreeView 实例
    QTreeView treeView;

    // 创建 QStandardItemModel 实例
    QStandardItemModel model(4, 2); // 4行2列

    // 向模型中添加数据
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 2; ++col) {
            QModelIndex index = model.index(row, col, QModelIndex());
            model.setData(index, QString("Row %1, Column %2").arg(row + 1).arg(col + 1));
        }
    }

    // 将模型设置为 QTreeView 的模型
    treeView.setModel(&model);

    // 设置表头可见
    treeView.header()->setVisible(true);

    // 显示 QTreeView
    treeView.show();

    return app.exec();
}