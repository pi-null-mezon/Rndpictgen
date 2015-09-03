#include <QImage>
#include <QString>
#include <QPainter>
#include <QPainterPath>
#include <windows.h>
#include <cstdio>
//---------------------------------------------------------------------------------

QColor qrandcolor();
qreal qfrand();
void randomPaint(QImage& input, uint triangles, uint ellipses, uint rectangles, uint circles, uint sizedivisor, bool contour);
void convertToGrayscale(QImage &input);
void applySaltPapper(QImage &input);
void applyNoise(QImage &input);
enum Primitives {RectanglePrimitive, TrianglePrimitive, EllipsePrimitive, CirclePrimitive};

//---------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    qsrand(GetTickCount()); // to seed pseudo-random number generator
    int m_exitCode;
    QString m_fileName("Default.png");
    int m_pictureWidth = 0;
    int m_pictureHeight = 0;
    uint m_rectNumber = 0;
    uint m_ellipseNumber = 0;
    uint m_triangleNumber = 0;
    uint m_circleNumber = 0;
    uint m_sizeDivisor = 0;
    bool f_grayscale = false;
    bool f_saltpepper = false;
    bool f_noise = false;
    bool f_contour = false;

    while( (--argc > 0) && ((*++argv)[0] == '-') ) { // a loop till argv[] ends
        char m_cmdOption = *++argv[0];
        switch (m_cmdOption)
        {
            case 'o':
                m_fileName = QString(++(*argv));
                break;
            case 'y':
                m_pictureHeight = QString(++(*argv)).toInt();
                break;
            case 'x':
                m_pictureWidth = QString(++(*argv)).toInt();
                break;
            case 't':
                m_triangleNumber = QString(++(*argv)).toUInt();
                break;
            case 'e':
                m_ellipseNumber = QString(++(*argv)).toUInt();
                break;
            case 'r':
                m_rectNumber = QString(++(*argv)).toUInt();
                break;
            case 'c':
                m_circleNumber = QString(++(*argv)).toUInt();
                break;
            case 'd':
                m_sizeDivisor = QString(++(*argv)).toUInt();
                break;
            case 'g':
                f_grayscale = true;
                break;
            case 's':
                f_saltpepper = true;
                break;
            case 'n':
                f_noise = true;
                break;
            case 'b':
                f_contour = true;
                break;
            case 'h':
                std::printf("%s v.%s, designed by %s\n"
                            " -h - force to show this help"
                            " -o[Ouput file name]\n"
                            " -y[Height] - desired picture height\n"
                            " -x[Width] - desired picture width\n"
                            " -t[Triangles] - desired quantity of primitives\n"
                            " -c[Circles] - desired quantity of primitives\n"
                            " -r[Rectangles] - desired quantity of primitives\n"
                            " -e[Ellipses] - desired quantity of primitives\n"
                            " -d[Divider] - smaller divider provides larger primitives\n"
                            " -n - force to apply additive noise\n"
                            " -s - force to apply salt and pepper interference\n"
                            " -g - force to generate grayscale picture\n"
                            " -b - force to add border line to primitives\n"
                            "Sources are available at https://github.com/pi-null-mezon/randpicture.git\n"
                            , QString(APP_NAME).toLocal8Bit().constData()
                            , QString(APP_VERS).toLocal8Bit().constData()
                            , QString(APP_DESIGNER).toLocal8Bit().constData());
                break;
        }
    }
    if((m_pictureHeight > 0) && (m_pictureWidth > 0) && (m_sizeDivisor > 0)) {
        QImage m_picture(m_pictureWidth, m_pictureHeight,  QImage::Format_RGB888);
        randomPaint(m_picture, m_triangleNumber, m_ellipseNumber, m_rectNumber, m_circleNumber, m_sizeDivisor, f_contour);
        if(f_saltpepper)
            applySaltPapper(m_picture);
        if(f_noise)
            applyNoise(m_picture);
        if(f_grayscale)
            convertToGrayscale(m_picture);
        if(m_picture.save(m_fileName)) {
            std::printf("Picture has been saved to a file: %s\n", m_fileName.toLocal8Bit().constData());
            m_exitCode = 0;
        } else {
            m_exitCode = -2;
        }
    } else {
        m_exitCode = -1;
    }
    return m_exitCode;
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
QColor qrandcolor()
{
    return QColor(qrand() % 256, qrand() % 256, qrand() % 256, qrand() % 256);
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
qreal qfrand()
{
    return ((qreal)qrand() / (RAND_MAX + 1.0));
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
QRectF randrect(qreal width, qreal height, qreal divisor)
{
    qreal x = ((2.0 * qfrand() - 1.0) * width/2.5);
    qreal y = ((2.0 * qfrand() - 1.0) * height/2.5);
    qreal w = ((qfrand() + 1.0) * width/divisor);
    qreal h = ((qfrand() + 1.0) * height/divisor);
    return QRectF(x,y,w,h);
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
QRectF randsquare(qreal width, qreal height, qreal divisor)
{
    qreal x = ((2.0 * qfrand() - 1.0) * width/2.5);
    qreal y = ((2.0 * qfrand() - 1.0) * height/2.5);
    qreal d = ((qfrand() + 1.0) * (width + height)/(2.5*divisor));
    return QRectF(x,y,d,d);
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void randomPaint(QImage& input, uint triangles, uint ellipses, uint rectangles, uint circles, uint sizedivisor, bool contour)
{
    QPainter painter(&input);
    painter.setRenderHint(QPainter::Antialiasing);
    if(!contour)
        painter.setPen(Qt::NoPen);
    else
        painter.setPen(Qt::SolidLine);
    painter.fillRect(input.rect(), qrandcolor());
    painter.translate(input.width() / 2.0, input.height() / 2.0);

    QList<QRectF> regions;
    QRectF rect;
    QPolygonF polygon;
    uint cycles;
    for(int key = 0; key <= 3; key++) {
        switch(key) {
        case RectanglePrimitive:
            cycles = rectangles;
            break;
        case TrianglePrimitive:
            cycles = triangles;
            break;
        case CirclePrimitive:
            cycles = circles;
            break;
        case EllipsePrimitive:
            cycles = ellipses;
            break;
        }
        for(uint i = 0; i < cycles; i++)
        {
            if(key == CirclePrimitive) {
                rect = randsquare(input.width(), input.height(), (qreal)sizedivisor);
            } else {
                rect = randrect(input.width(), input.height(), (qreal)sizedivisor);
            }
            QPainterPath path;
            switch(key) {
            case RectanglePrimitive:
                path.addRect(rect);;
                break;
            case TrianglePrimitive:
                polygon.clear();
                polygon << QPointF((rect.topLeft()+rect.topRight())/2.0)
                        << QPointF(rect.bottomLeft())
                        << QPointF(rect.bottomRight())
                        << QPointF((rect.topLeft()+rect.topRight())/2.0);
                path.addPolygon(polygon);
                break;
            case CirclePrimitive:
                path.addEllipse(rect);
                break;
            case EllipsePrimitive:
                path.addEllipse(rect);
                break;
            }
            painter.setBrush(qrandcolor());
            painter.save();
            painter.rotate(360.0 * qfrand());
            painter.drawPath(path);
            painter.restore();
        }
    }
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void convertToGrayscale(QImage &input)
{
    uchar *pt;
    int pos;
    uchar mean;
    for(int i = 0; i < input.height(); i++) {
        pt = input.scanLine(i);
        for(int j = 0; j < input.width(); j++) {
            pos = 3*j;
            mean = (uchar)(((qreal)pt[pos] + pt[pos+1]+ pt[pos+2])/3.0);
            pt[pos] = mean;
            pt[pos+1] = mean;
            pt[pos+2] = mean;
        }
    }
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void applySaltPapper(QImage &input)
{
    uchar *pt;
    int dice;
    for(int i = 0; i < input.height(); i++) {
        pt = input.scanLine(i);
        for(int j = 0; j < input.width(); j++) {
            dice = qrand();
            if((dice % 53) == 0) {
                pt[3*j] = 0;
                pt[3*j+1] = 0;
                pt[3*j+2] = 0;
            } else if((dice % 59) == 0) {
                pt[3*j] = 255;
                pt[3*j+1] = 255;
                pt[3*j+2] = 255;
            }
        }
    }
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
void applyNoise(QImage &input)
{
    uchar *pt;
    int dice;
    int pos;
    int temp;
    for(int i = 0; i < input.height(); i++) {
        pt = input.scanLine(i);
        for(int j = 0; j < input.width(); j++) {
            pos = 3*j;
            dice = qrand() % 37;
            temp = (int)pt[pos] + dice;
            pt[pos] = (temp > 255) ? 255 : temp;
            temp = (int)pt[pos+1] + dice;
            pt[pos+1] = (temp > 255) ? 255 : temp;
            temp = (int)pt[pos+2] + dice;
            pt[pos+2] = (temp > 255) ? 255 : temp;
        }
    }
}
//---------------------------------------------------------------------------------




