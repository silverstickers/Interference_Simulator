#include "controller.h"
#include <QtMath>

Controller::Controller(QObject *parent) :
    QObject(parent)
  , master()
{
    master.moveToThread(&masterThread);
    connect(this, &Controller::calculateRender, &master, &RenderingMaster::startRender);
    connect(&master, &RenderingMaster::imageRendered, this, &Controller::handleNewImage);
    masterThread.start();
}

Controller::~Controller()
{
    masterThread.quit();
    masterThread.wait();
}

QImage Controller::image()
{
    return m_image;
}

void Controller::startCalculation()
{
    double xrange = static_cast<double>(m_xrange)/static_cast<double>(m_zoom);
    double yrange = static_cast<double>(m_yrange)/static_cast<double>(m_zoom);

    emit calculateRender(&m_image, m_xcenter, m_ycenter, xrange, yrange, m_width, m_height, m_angle, m_count, m_spacing);
}

void Controller::zoomIn()
{
    m_zoom *= ZOOMFACTOR;
    int xoffset = m_width*(ZOOMFACTOR-1)/(2*ZOOMFACTOR);
    int yoffset = m_height*(ZOOMFACTOR-1)/(2*ZOOMFACTOR);
    QImage tempImage = m_image.copy(xoffset, yoffset, m_width/ZOOMFACTOR, m_height/ZOOMFACTOR);
    handleNewImage(tempImage.scaled(m_width, m_height));
    startCalculation();
}

void Controller::zoomOut()
{
    m_zoom /= ZOOMFACTOR;
    int xoffset = -m_width*(ZOOMFACTOR-1)/(2);
    int yoffset = -m_height*(ZOOMFACTOR-1)/(2);
    QImage tempImage = m_image.copy(xoffset, yoffset, m_width*ZOOMFACTOR, m_height*ZOOMFACTOR);
    handleNewImage(tempImage.scaled(m_width, m_height));
    startCalculation();
}

void Controller::moveLeft()
{
    m_xcenter -= m_xrange / m_zoom / TRANSLATIONFRACTION;
    int xoffset = -m_width / TRANSLATIONFRACTION;
    int yoffset = 0;
    QImage tempImage = m_image.copy(xoffset, yoffset, m_width, m_height);
    handleNewImage(tempImage);
    startCalculation();
}

void Controller::moveRight()
{
    m_xcenter += m_xrange / m_zoom / TRANSLATIONFRACTION;
    int xoffset = m_width / TRANSLATIONFRACTION;
    int yoffset = 0;
    QImage tempImage = m_image.copy(xoffset, yoffset, m_width, m_height);
    handleNewImage(tempImage);
    startCalculation();
}

void Controller::moveUp()
{
    m_ycenter += m_yrange / m_zoom / TRANSLATIONFRACTION;
    int xoffset = 0;
    int yoffset = -m_height / TRANSLATIONFRACTION;
    QImage tempImage = m_image.copy(xoffset, yoffset, m_width, m_height);
    handleNewImage(tempImage);
    startCalculation();
}

void Controller::moveDown()
{
    m_ycenter -= m_yrange / m_zoom / TRANSLATIONFRACTION;
    int xoffset = 0;
    int yoffset = m_height / TRANSLATIONFRACTION;
    QImage tempImage = m_image.copy(xoffset, yoffset, m_width, m_height);
    handleNewImage(tempImage);
    startCalculation();
}

void Controller::angleSlider(double angle)
{
    m_angle = static_cast<double>(angle);
    startCalculation();
}

void Controller::numberSlider(int number)
{
    m_count = number;
    startCalculation();
}

void Controller::spacingSlider(double spacing)
{
    m_spacing = static_cast<double>(spacing*0.001);
    startCalculation();
}


void Controller::handleNewImage(const QImage &image)
{
    if (image != m_image) {
        m_image = image;
        emit imageRendered();
    }
}



