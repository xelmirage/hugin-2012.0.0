#ifndef USAGECHART_H
#define USAGECHART_H

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>
//#include <QGraphicsView>
//#include "ui_multiview.h"
#include <QtCore/QDebug>
#include <QtCore/QAbstractListModel>
#include <QtCore/QList>
#include <QtCore/QPoint>

#include <QtGui/QPixmap>
#include <QtGui/QtGui>
#include <QtGui/QBrush>
#include <QtGui/QPen>
#include <QtGui/QLabel>
#include<QtCore\QTextCodec>

/*******************************************************/
/** UsageChart can only accept data from 0 to 100 now **/
/*****************************************************/

/*!
  Macro

  */
#define TEXT_BOX_SPACE 5
#define X_INCREMENT 5
#define POINT_RADIUS 3
class UsageChart : public QWidget
{
Q_OBJECT
public:
    explicit UsageChart(QWidget *parent = 0);

public slots:
    void addData(qreal data);

    void setUseAntialiasing(bool use)
    {
        m_bUseAntialiasing=use;
        update();
    }

    bool getUseAntialiasing() const
    {
        return m_bUseAntialiasing;
    }

    void setHorizontalLineColor(const QColor& clr)
    {
        m_hLineClr=clr;
        update();
    }

    void setShowPoint(bool show)
    {
        m_bShowPoint=show;
        update();
    }

    void setTitle(const QString& str)
    {
        m_strTitle=str;
        update();
    }

protected:
    void paintEvent(QPaintEvent *);

    void resizeEvent(QResizeEvent *)
    {
        updateVector();
    }

    QSize sizeHint() const
    {
        return QSize(350,350);

    }


    QSize minimumSizeHint() const
    {
        return QSize(280,280);
    }

    /*!

      painting used private functions
      */
private:
    void resetVariables(QPainter* painter);
    void drawBackground(QPainter* painter);
    void drawBox(QPainter* painter);
    void drawText(QPainter* painter);
    void drawGraph(QPainter* painter);
    void drawTitle(QPainter* painter);



private:
    qreal            m_leftSpace;
    qreal            m_topSpace;
    QVector<qreal>   m_dataVec;
    QVector<QPointF> m_potVec;
    bool             m_bUseAntialiasing;
    bool             m_bShowHLine;
    bool             m_bShowPoint;
    QRectF           m_topRect;
    QRectF           m_bottomRect;
    QRectF           m_boxRect;
    QColor           m_hLineClr;
    QString          m_strTitle;

    /*!
      private utility functions
      */
    void initVariables();

    void updateVector();


};

#endif // USAGECHART_H
