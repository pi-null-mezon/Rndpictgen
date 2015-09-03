#include <QImage>
#include <QString>
#include <QPainter>
#include <QPainterPath>
#include <windows.h>

QColor qrandcolor();
qreal qfrand();
QImage randompaint(const QImage& input, uint triangles, uint ellipses, uint rectangles, uint circles, uint fract, uint maxiter);
void convertToGrayscale(QImage &input);

int main(int argc, char *argv[])
{
    qsrand(GetTickCount());
    int m_exitCode;
    char m_cmdOption;
    QString m_fileName("Default.png");
    int m_pictureWidth = 512;
    int m_pictureHeight = 512;
    uint m_rectNumber = 0;
    uint m_ellipseNumber = 0;
    uint m_triangleNumber = 0;
    uint m_circleNumber = 0;
    uint m_iterationNumber = 128;
    uint m_objectSize = 32;
    bool f_grayscale = false;

    while( (--argc > 0) && ((*++argv)[0] == '-') ) { // a loop till argv[] ends
        m_cmdOption = *++argv[0];
        switch (m_cmdOption)
        {
            case 'o':
                m_fileName = QString(++(*argv));
                break;
            case 'h':
                m_pictureHeight = QString(++(*argv)).toInt();
                break;
            case 'w':
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
            case 'i':
                m_iterationNumber = QString(++(*argv)).toUInt();
                break;
            case 's':
                m_objectSize = QString(++(*argv)).toUInt();
                break;
            case 'g':
                f_grayscale = true;
                break;
        }
    }
    if((m_pictureHeight > 0) && (m_pictureWidth > 0) && (m_objectSize > 0)) {
        QImage m_picture(m_pictureHeight, m_pictureWidth, QImage::Format_RGB888);
        m_picture = randompaint(m_picture, m_triangleNumber, m_ellipseNumber, m_rectNumber, m_circleNumber, m_objectSize, m_iterationNumber);
        if(f_grayscale) {
            convertToGrayscale(m_picture);
        }
        if(m_picture.save(m_fileName)) {
            qWarning("Picture has been saved to a file: %s", m_fileName.toLocal8Bit().constData());
            qWarning("Image dimensions: height %d x width %d", m_pictureHeight, m_pictureWidth);
            m_exitCode = 0;
        } else {
            m_exitCode = -2;
        }
    } else {
        m_exitCode = -1;
    }

    return m_exitCode;
}

QColor qrandcolor()
{
    return QColor(qrand() % 256, qrand() % 256, qrand() % 256, qrand() % 256);
}

qreal qfrand()
{
    return ((qreal)qrand() / (RAND_MAX + 1.0));
}

QRectF randrect(qreal width, qreal height, qreal fraction)
{
    qreal x = ((2.0 * qfrand() - 1.0) * width/2.5);
    qreal y = ((2.0 * qfrand() - 1.0) * height/2.5);
    qreal w = ((qfrand() + 1.0) * width/fraction);
    qreal h = ((qfrand() + 1.0) * height/fraction);
    return QRectF(x,y,w,h);
}
QRectF randsquare(qreal width, qreal height, qreal fraction)
{
    qreal x = ((2.0 * qfrand() - 1.0) * width/2.5);
    qreal y = ((2.0 * qfrand() - 1.0) * height/2.5);
    qreal d = ((qfrand() + 1.0) * (width + height)/(2.5*fraction));
    return QRectF(x,y,d,d);
}

QImage randompaint(const QImage& input, uint triangles, uint ellipses, uint rectangles, uint circles, uint fract, uint maxiter)
{
    int steps = maxiter;
    QImage output(input);
    QPainter painter(&output);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    painter.fillRect(output.rect(), qrandcolor());
    painter.translate(input.width() / 2.0, input.height() / 2.0);

    QList<QRectF> regions;
    QRectF newRect;
    bool intersection;
    {
        for(uint i = 0; i < rectangles; i++)
        {
            do {
                intersection = false;
                newRect = randrect(input.width(), input.height(), (qreal)fract);
                steps--;
                qWarning("Available iterations remain: %d", steps);
                foreach(const QRectF& oldRect, regions) {
                    if(oldRect.intersects(newRect)) {
                        intersection = true;
                        break;
                    }
                }
            } while( (intersection == true) && (steps > 0) );
            regions.append(newRect);

            QPainterPath path;
            path.addRect(newRect);
            painter.setBrush(qrandcolor());

            painter.save();
            painter.rotate(360.0 * qfrand());
            painter.drawPath(path);
            painter.restore();
        }
    }
    for(uint i = 0; i < triangles; i++)
    {
        do {
            intersection = false;
            newRect = randrect(input.width(), input.height(), (qreal)fract);
            steps--;
            qWarning("Available iterations remain: %d", steps);
            foreach(const QRectF& oldRect, regions) {
                if(oldRect.intersects(newRect)) {
                    intersection = true;
                    break;
                }
            }
        } while((intersection == true) && (steps > 0));
        regions.append(newRect);

        QPointF p1((newRect.topLeft()+newRect.topRight())/2.0);
        QPointF p2(newRect.bottomLeft());
        QPointF p3(newRect.bottomRight());
        QPolygonF triangle;
        triangle << p1 << p2 << p3;

        QPainterPath path;
        path.addPolygon(triangle);
        painter.setBrush(qrandcolor());

        painter.save();
        painter.rotate(360.0 * qfrand());
        painter.drawPath(path);
        painter.restore();
    }
    for(uint i = 0; i < ellipses; i++)
    {
        do {
            intersection = false;
            newRect = randrect(input.width(), input.height(), (qreal)fract);
            steps--;
            qWarning("Available iterations remain: %d", steps);
            foreach(const QRectF& oldRect, regions) {
                if(oldRect.intersects(newRect)) {
                    intersection = true;
                    break;
                }
            }
        } while((intersection == true) && (steps > 0));
        regions.append(newRect);

        QPainterPath path;
        path.addEllipse(newRect);
        painter.setBrush(qrandcolor());

        painter.save();
        painter.rotate(360.0 * qfrand());
        painter.drawPath(path);
        painter.restore();
    }
    for(uint i = 0; i < circles; i++)
    {
        do {
            intersection = false;
            newRect = randsquare(input.width(), input.height(), (qreal)fract);
            steps--;
            qWarning("Available iterations remain: %d", steps);
            foreach(const QRectF& oldRect, regions) {
                if(oldRect.intersects(newRect)) {
                    intersection = true;
                    break;
                }
            }
        } while((intersection == true) && (steps > 0));
        regions.append(newRect);

        QPainterPath path;
        path.addEllipse(newRect);
        painter.setBrush(qrandcolor());

        painter.save();
        painter.rotate(360.0 * qfrand());
        painter.drawPath(path);
        painter.restore();
    }
    return output;
}

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




