#include "testWidget.h"

testWidget::testWidget(QWidget *parent) {
    parent = nullptr;
    std::cout << "testWidget is generated" << std::endl;
}

testWidget::~testWidget() noexcept {

}

void testWidget::addButton() {
    var+=1;
    std::cout << var << std::endl;
}

double testWidget::getVar() {
    return var;
}

double testWidget::var = 0;