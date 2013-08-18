#ifndef MULTIVIEW_H
#define MULTIVIEW_H

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

#include <vector>
#include <string>
#include <sstream>


using namespace std;


class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QScrollArea;
class QListView;
class QMimeData;


class ImgViewer : public QLabel
{
	Q_OBJECT
public:
    ImgViewer(QLabel *parent = 0);

	QPen pen;
	double scale;

    void paintEvent(QPaintEvent *event);

};

class IconList : public QAbstractListModel
{
	Q_OBJECT

public:
    IconList(QObject *parent = 0);
     
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    
    //no use now
    QMimeData *mimeData(const QModelIndexList &indexes) const;

    void addPiece(const QPixmap &pixmap, const QPoint &location);
 
    QSize _isize;
    QSize _gsize;
    int _space;

     QList<QImage*> imglist;

private:
     QList<QPoint> locations;
     QList<QPixmap> pixmaps;

};

class ImgPreview : public QListView//QWidget
{
	Q_OBJECT
public:
	ImgPreview(QWidget *parent = 0);

	std::vector<QImage*> imglist;
    //QImage imgarray[20];
    IconList* iconlist;
	//std::vector<QLabel*> imgLabel; 
    //QLabel* imgLabel[5];
    //QLabel* activeLabel;
    void ReadImages(QString sdir);
    void GetCurrentIndex(QModelIndex * index);

private slots:
    void setActiveLabel();

protected:
    //void mouseDoubleClickEvent ( QMouseEvent * event );

};


class multiview : public QMainWindow
{
	Q_OBJECT

public:
	multiview(QWidget *parent = 0, Qt::WFlags flags = 0);
	~multiview();

private slots:

    void newFile();
     void save();
     void print();
     void undo();
     void about();
     void setNewImage(QString  item);

    void zoomIn();
    void zoomOut();

	void process();


private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createDockWindows();
	

	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);
	void tick(QString message);
	int execexternal(QProcess* myprocess,QString command,QString tickmessage);
	void push_message(QString message);
	


	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;

	//actions must be add into menu to take its function!!

     QAction *newFileAct;
     QAction *saveAct;
     QAction *printAct;
     QAction *undoAct;
     QAction *aboutAct;
     QAction *aboutQtAct;
     QAction *quitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
	QAction *processAct;


private:
	//Ui::multiviewClass ui;
	///QLineEdit* lineEdit_4;

	ImgViewer *mainview;
    QTextEdit *textEdit;
	ImgPreview* preViewer;
    QScrollArea *scrollArea;
	QListWidget* m_pListWidget;
	double scaleFactor;

signals:
    void outlogtext(QString ver);
private slots:
    void outlog();
	void count_time();
    
private:
	QTimer* timer ;
	QLineEdit*  lineEdit_4;
	QTextCursor* cursor; 
	QProgressBar* pBar;
	
	
    QProcess *myprocess;
	int time_count;
	QString run_time;
	int tick_max;
	int previewSize;
	QString fileName,gpsfileName,sdir; 


};

#endif // MULTIVIEW_H
