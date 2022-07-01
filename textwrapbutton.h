#ifndef TEXTWRAPBUTTON_H
#define TEXTWRAPBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QLabel>

class TextWrapButton : public QPushButton
{
public:
    TextWrapButton(const QString &text, QWidget *parent = nullptr);
    TextWrapButton(QWidget *parent = nullptr);
    void setText(const QString &);
    QString text() const;

    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);

private:
    QLabel *label;
};

#endif // TEXTWRAPBUTTON_H
