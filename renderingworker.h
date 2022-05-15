#ifndef RENDERINGWORKER_H
#define RENDERINGWORKER_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include <complex>
#include <iostream>

class RenderingMaster;

using complex = std::complex<double>;

Q_DECLARE_METATYPE(double);

class RenderingWorker : public QRunnable
{
public:
    RenderingWorker(RenderingMaster *master, complex corner, complex step,
                     int width, int height, int yPos,
                     double angle, int count, double spacing, bool logScale, volatile bool *stopped);

private:
    void run() override;

    RenderingMaster *m_master;
    complex m_corner, m_step;
    int m_width, m_height, m_yPos, m_count;

    double m_angle;
    double m_spacing;
    bool m_logScale;

    std::vector<std::complex<double>> m_sensor_positions;

    volatile bool *stopped;
};

#endif // RENDERINGWORKER_H
