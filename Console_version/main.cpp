#include <QImage>
#include <QString>
#include <QPainter>
#include <QPainterPath>
#include <QFont>
#include <QTime>
#include <QApplication>
#include <cstdio>

#define DIVISOR 1.1

QColor qrandcolor();
inline qreal qfrand();
QColor randomFill(QImage& input);
void randomPaint(QImage& input, uint triangles, uint ellipses, uint rectangles, uint circles, uint sizedivisor, bool contour, QColor contourcolor);
void randomPaint(QImage& input, const std::vector<char> & vS, const std::vector<int> & vN, uint sizedivisor, bool contour, QColor contourcolor);
void makeInfoStamp(QImage& input, uint triangles, uint ellipses, uint rectangles, uint circles, const std::vector<char> & vS, const std::vector<int> & vN);
void convertToGrayscale(QImage &input);
void applySaltPapper(QImage &input, uint step);
void applyNoise(QImage &input, uint deviation);
void applyRadialGradient(QImage &input);
QColor oppositecolor(const QColor& probe);
enum Primitives {TrianglePrimitive, RectanglePrimitive, CirclePrimitive, EllipsePrimitive};

//---------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc, argv); //this instance and followed call are provided here only for QFont, because nonGui application can not handle this class (
    app.processEvents();

    qsrand((uint)QTime::currentTime().msecsSinceStartOfDay()); // to seed pseudo-random number generator
    int m_exitCode;
    QString m_fileName("default.png");
    int m_pictureWidth = 1280;
    int m_pictureHeight = 720;
    uint m_rectNumber = 5;
    uint m_ellipseNumber = 3;
    uint m_triangleNumber = 7;
    uint m_circleNumber = 2;
    uint m_sizeDivisor = 31;
    uint m_saltpepperStep = 171;
    uint m_noiseDeviation = 11;
    bool f_grayscale = false;
    bool f_contour = false;
    bool f_stamp = false;
    bool f_gradient = false;

    std::vector<char> v_symbol;
    std::vector<int>  v_number;


    while( (--argc > 0) && ((*++argv)[0] == '-') ) { // a loop till argv[] ends
        char m_cmdOption = *++argv[0];
        switch (m_cmdOption)
        {
            case 'l':
                v_symbol.push_back(*++(*argv));
                v_number.push_back( QString(++(*argv)).toInt() );
                break;
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
                m_saltpepperStep = QString(++(*argv)).toUInt();
                break;
            case 'n':
                m_noiseDeviation = QString(++(*argv)).toUInt();
                break;
            case 'b':
                f_contour = true;
                break;
            case 'i':
                f_stamp = true;
                break;
            case 'u':
                f_gradient = true;
                break;
            case 'h':
                std::printf("%s v.%s, designed by %s\n"
                            " -h - force to show this help\n"
                            " -o[Ouput file name]\n"
                            " -y[Height] - desired picture height, default %d\n"
                            " -x[Width] - desired picture width, default %d\n"
                            " -t[Triangles] - desired quantity of primitives, default %d\n"
                            " -c[Circles] - desired quantity of primitives, default %d\n"
                            " -r[Rectangles] - desired quantity of primitives, default %d\n"
                            " -e[Ellipses] - desired quantity of primitives, default %d\n"
                            " -l[Letter][Quantity] - desired Letter and desired Quantity\n"
                            " -d[Divider] - smaller divider provides larger primitives, default %d\n"
                            " -n[Deviation] - force to apply additive noise, default %d\n"
                            " -s[Step] - force to apply salt and pepper interference, default %d\n"
                            " -u - force to add radial gradient shade effect, default %s\n"
                            " -g - force to generate grayscale picture, default %s\n"
                            " -b - force to add border line to primitives, default %s\n"
                            " -i - force to make an info stamp on picture, default %s\n"
                            "Sources are available at https://github.com/pi-null-mezon/randpicture.git\n"
                            , QString(APP_NAME).toLocal8Bit().constData()
                            , QString(APP_VERS).toLocal8Bit().constData()
                            , QString(APP_DESIGNER).toLocal8Bit().constData()
                            , m_pictureHeight
                            , m_pictureWidth
                            , m_triangleNumber
                            , m_circleNumber
                            , m_rectNumber
                            , m_ellipseNumber
                            , m_sizeDivisor
                            , m_noiseDeviation
                            , m_saltpepperStep
                            , f_gradient ? "true":"false"
                            , f_grayscale ? "true":"false"
                            , f_contour ? "true":"false"
                            , f_stamp ? "true":"false");
                return 1;
        }
    }

    if((m_pictureHeight > 0) && (m_pictureWidth > 0) && (m_sizeDivisor > 0)) {
        QImage m_picture(m_pictureWidth, m_pictureHeight,  QImage::Format_RGB888);
        QColor cntcolor = randomFill(m_picture);
        randomPaint(m_picture, m_triangleNumber, m_ellipseNumber, m_rectNumber, m_circleNumber, m_sizeDivisor, f_contour, cntcolor);
        randomPaint(m_picture, v_symbol, v_number, m_sizeDivisor, f_contour, cntcolor);
        if(f_gradient)
            applyRadialGradient(m_picture);
        if(m_saltpepperStep > 0)
            applySaltPapper(m_picture, m_saltpepperStep);
        if(m_noiseDeviation > 0)
            applyNoise(m_picture, m_noiseDeviation);
        if(f_stamp)
            makeInfoStamp(m_picture, m_triangleNumber, m_ellipseNumber, m_rectNumber, m_circleNumber, v_symbol, v_number);
        if(f_grayscale)
            convertToGrayscale(m_picture);
        if(m_picture.save(m_fileName)) {
            std::printf("Picture has been saved to a file: %s\n", m_fileName.toLocal8Bit().constData());
            m_exitCode = 0;
        } else {
            std::printf("Can not save picture into file");
            m_exitCode = -2;
        }
    } else {
        std::printf("Abort: zero picture size or zero divider");
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
QColor oppositecolor(const QColor& probe)
{
    return QColor((probe.red() + 128)%256, (probe.green() + 128)%256, (probe.green() + 128)%256);
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
qreal qfrand()
{
    return (qreal)qrand()/RAND_MAX;
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
QRectF randrect(qreal width, qreal height, qreal divisor)
{
    qreal w = ((qfrand() + 0.5) * width/divisor);
    qreal h = ((qfrand() + 0.5) * height/divisor);
    return QRectF( - w/2.0, - h/2.0, w, h);
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
QRectF randsquare(qreal width, qreal height, qreal divisor)
{
    qreal d = ((qfrand() + 0.5) * (width + height)/(2.0*divisor));
    return QRectF( - d/2.0, - d/2.0, d, d);
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

QColor randomFill(QImage& input)
{
    QPainter painter(&input);
    QColor color = qrandcolor();
    painter.fillRect(input.rect(), color);
    return oppositecolor(color);
}

//---------------------------------------------------------------------------------
void randomPaint(QImage& input, uint triangles, uint ellipses, uint rectangles, uint circles, uint sizedivisor, bool contour, QColor contourcolor)
{
    QPainter painter(&input);
    painter.setRenderHint(QPainter::Antialiasing);
    if(!contour)
        painter.setPen(Qt::NoPen);
    else {
        painter.setPen(contourcolor);
    }
    painter.translate(input.width() / 2.0, input.height() / 2.0);

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
            painter.translate((qfrand() - 0.5)*input.width()/DIVISOR, (qfrand() - 0.5)*input.height()/DIVISOR);
            painter.rotate(360.0 * qfrand());
            painter.drawPath(path);
            painter.restore();
        }
    }
}
//---------------------------------------------------------------------------------

void randomPaint(QImage& input, const std::vector<char> & vS, const std::vector<int> & vN, uint sizedivisor, bool contour, QColor contourcolor)
{
    QPainter painter(&input);
    painter.setRenderHint(QPainter::Antialiasing);
    if(!contour)
        painter.setPen(Qt::NoPen);
    else {
        painter.setPen(contourcolor);
    }
    painter.translate(input.width() / 2.0, input.height() / 2.0);

    QRectF rect;

    for(uint i = 0; i < vS.size(); i++)
    {
        for(uint j = 0; j < vN[i]; j++)
        {
            rect = randrect(input.width(), input.height(), (qreal)sizedivisor);
            QPainterPath path;
            QFont font("GOST A", (rect.height()+rect.width())/2, QFont::DemiBold, true);
            path.addText(rect.bottomLeft(), font, QString(vS[i]));

            painter.setBrush(qrandcolor());
            painter.save();
            painter.translate((qfrand() - 0.5)*input.width()/DIVISOR, (qfrand() - 0.5)*input.height()/DIVISOR);
            painter.rotate(360.0 * qfrand());
            painter.drawPath(path);
            painter.restore();
        }
    }
}

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
void applySaltPapper(QImage &input, uint step)
{
    uchar *pt;
    int dice;
    for(int i = 0; i < input.height(); i++) {
        pt = input.scanLine(i);
        for(int j = 0; j < input.width(); j++) {
            dice = qrand();
            if((dice % step) == 0) {
                pt[3*j] = 0;
                pt[3*j+1] = 0;
                pt[3*j+2] = 0;
            } else if((dice % (step + 1)) == 0) {
                pt[3*j] = 255;
                pt[3*j+1] = 255;
                pt[3*j+2] = 255;
            }
        }
    }
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void applyNoise(QImage &input, uint deviation)
{
    uchar *pt;
    int dice;
    int pos;
    int temp;
    for(int i = 0; i < input.height(); i++) {
        pt = input.scanLine(i);
        for(int j = 0; j < input.width(); j++) {
            pos = 3*j;
            dice = qrand() % deviation;
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

//---------------------------------------------------------------------------------
void applyRadialGradient(QImage& input)
{
    QPainter painter(&input);
    QColor pColor = qrandcolor();
    pColor.setAlpha((1.0 + qfrand())* 127);
    QColor cColor(pColor);
    cColor.setAlpha(0);
    QPointF cPoint(input.width()/2.0, input.height()/2.0);
    QPointF fPoint(qfrand()*input.width(), qfrand()*input.height());
    QRadialGradient gradient(cPoint, (input.width()+input.height())/2.0, fPoint);
    gradient.setColorAt(0.0, cColor);
    gradient.setColorAt(1.0, pColor);
    painter.fillRect(input.rect(), QBrush(gradient));
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void makeInfoStamp(QImage& input, uint triangles, uint ellipses, uint rectangles, uint circles, const std::vector<char> &vS, const std::vector<int> &vN)
{
    QPainter painter(&input);
    QPen pen(Qt::NoBrush, 0.75);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setBrush(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    qreal pointSize = input.height()/32.0;
    QFont font("Tahoma", pointSize, QFont::DemiBold, true);

    qreal startX = 0.5 * pointSize;
    qreal startY = 1.5 * pointSize;
    QPainterPath path;
    for(int key = 0; key < 4; key++) {
        switch(key) {
            case TrianglePrimitive:
                path.addText(startX, startY, font, "▲: " + QString::number(triangles));
                break;
            case RectanglePrimitive:
                path.addText(startX, startY, font, "■:" + QString::number(rectangles));
                break;
            case CirclePrimitive:
                path.addText(startX, startY, font, "●:" + QString::number(circles));
                break;
            case EllipsePrimitive:
                path.addText(startX, startY, font, "ellipse:" + QString::number(ellipses));
                break;
            }
        startY += 1.5 * pointSize;
    }
    for(int i = 0; i < vS.size(); i++) {
        path.addText(startX, startY, font, "'" + QString(vS[i]) + "':" + QString::number(vN[i]));
        startY += 1.5 * pointSize;
    }
    painter.drawPath(path);
}
//---------------------------------------------------------------------------------


