
#include "multiview.h"
#include <QtGui/QImage>
#include <QtGui/QMessageBox>

multiview::multiview(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	
	//ui.setupUi(this);
	char arg1[]="*.jpg";
	char arg2[]="-o out.pto";
	char arg3[]="asdfaw";
	mainview = new ImgViewer; 

	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(mainview);

	//setCentralWidget(mainview);
	setCentralWidget(scrollArea);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createDockWindows();

	setWindowTitle(tr("Dock Widgets"));

	resize(640, 480);
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);




}

multiview::~multiview()
{

}

void multiview::createActions()
{
	newFileAct = new QAction(QIcon("images/new.png"), tr("&New Letter"),this);
	newFileAct->setShortcut(tr("Ctrl+N"));
	newFileAct->setStatusTip(tr("Create a new form letter"));
	connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));

	saveAct = new QAction(QIcon("images/save.png"), tr("&Save..."), this);
	saveAct->setShortcut(tr("Ctrl+S"));
	saveAct->setStatusTip(tr("Save the current form letter"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	printAct = new QAction(QIcon("images/print.png"), tr("&Print..."), this);
	printAct->setShortcut(tr("Ctrl+P"));
	printAct->setStatusTip(tr("Print the current form letter"));
	connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

	undoAct = new QAction(QIcon("images/undo.png"), tr("&Undo"), this);
	undoAct->setShortcut(tr("Ctrl+Z"));
	undoAct->setStatusTip(tr("Undo the last editing action"));
	connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

	quitAct = new QAction(tr("&Quit"), this);
	quitAct->setShortcut(tr("Ctrl+Q"));
	quitAct->setStatusTip(tr("Quit the application"));
	connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


	zoomInAct = new QAction(tr("Zoom &In (2%)"), this);
	zoomInAct->setShortcut(tr("Ctrl+="));
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(tr("Zoom &Out (2%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

}

void multiview::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newFileAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(quitAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(undoAct);

	//actions must be add into menu to take its function!!
	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void multiview::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newFileAct);
	fileToolBar->addAction(saveAct);
	fileToolBar->addAction(printAct);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(undoAct);
}

void multiview::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void multiview::createDockWindows()
{
	QDockWidget *dock = new QDockWidget(tr("Command window"), this);
	dock->setAllowedAreas(  Qt::BottomDockWidgetArea 
		| Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	preViewer = new ImgPreview(dock);
	preViewer->setParent(this);

	//textEdit = new QTextEdit(dock);
	//   QTextCursor cursor(textEdit->textCursor());
	//   cursor.movePosition(QTextCursor::Start);

	//cursor.insertBlock();
	//   cursor.insertText("This is command window");
	//   cursor.insertBlock();
	//   cursor.insertText("Input your command here!");

	//   dock->setWidget(textEdit);
	//textEdit->setMinimumHeight(100);
	//textEdit->adjustSize();

	dock->setWidget(preViewer);
	//preViewer->setMaximumHeight(400);
	//preViewer->adjustSize();

	addDockWidget(Qt::BottomDockWidgetArea, dock);
	viewMenu->addAction(dock->toggleViewAction());
	connect(preViewer, SIGNAL(doubleClicked(const QModelIndex &)),
		this, SLOT(setNewImage()));


	//osgqtView = new ViewerQT;
	////osgqtView = new CompositeViewerQT;

	//   dock->setWidget(osgqtView);
	//addDockWidget(Qt::RightDockWidgetArea, dock);
	//   viewMenu->addAction(dock->toggleViewAction());

	//   //connect(paragraphsList, SIGNAL(currentTextChanged()),
	//   //        this, SLOT(addParagraph(const QString &)));
	////InitOsg();

} 

//-------------------slots

void multiview::newFile()
{
	//   QString fileName = QFileDialog::getOpenFileName(this,
	//                                   tr("Open File"), QDir::currentPath());


	//QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", 
	//	"Images (*.png *.tif *.jpg)" );


	//   if (!fileName.isEmpty()) 
	//{
	//	QImage image(fileName);

	//       if (image.isNull()) 
	//	{
	//           QMessageBox::information(this, tr("Image Viewer"),
	//                                    tr("Cannot load %1.").arg(fileName));
	//           return;
	//       }
	//       mainview->setPixmap(QPixmap::fromImage(image));
	//       scaleFactor = 1.0;

	//       printAct->setEnabled(true);
	//       //updateActions();
	//       zoomInAct->setEnabled(true);
	//       zoomOutAct->setEnabled(true);
	//           
	//	mainview->adjustSize();


	//}

	//   if(preViewer)
	//   {
	//       preViewer->ReadImages();

	//   }
	int i;


	QStringList    fileNameList;
	QString fileName; 
	QFileDialog* fd = new QFileDialog(this);//�����Ի���
	//fd->resize(240,320);    //������ʾ�Ĵ�С
	fd->setFilter( "Images (*.png *.tif *.jpg)"); //�����ļ�������
	fd->setViewMode(QFileDialog::List);  //�������ģʽ���� �б�list�� ģʽ�� ��ϸ��Ϣ��detail�����ַ�ʽ
	//fd->setMode( QFileDialog::ExistingFiles ); 
	fd->setFileMode(QFileDialog::ExistingFiles);
	if ( fd->exec() == QDialog::Accepted )   //����ɹ���ִ��
	{
		fileNameList = fd->selectedFiles();      //�����ļ��б������
		fileName = fileNameList[0];            //ȡ��һ���ļ���
	}
	else
		fd->close();

	QString argnames;
	argnames.clear();

		
	//	char * ch=(char*)str.c_str();
	//QMessageBox::information(NULL, "Title",ch, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	

	h.pto_gen(fileNameList,"qout.pto");
	h.cpfind("qout.pto","cpout.pto");

	//h.readData("cpout.pto");
	h.loadProject("cpout.pto");



	//h.cpoints=h.pano.getCtrlPoints();




	//char *  args[]={"","-oout.pto -p2","*.jpg"};//����ط�����֣�Ҫ���һ���ղ���������-o���治���пո�
	//ptogen::ptogen_main(3,args);

	//char * args2[]={"","out.pto","-o out2.pto"};
	//cpfind::cpfind_main(3,args2);







}

void multiview::about()
{
	QMessageBox::about(this, tr("About Dock Widgets"),
		tr("The <b>Dock Widgets</b> example demonstrates how to "
		"use Qt's dock widgets. You can enter your own text, "
		"click a customer to add a customer name and "
		"address, and click standard paragraphs to add them."));
}

void multiview::print()
{
	//QTextDocument *document = textEdit->document();
	//QPrinter printer;

	//QPrintDialog *dlg = new QPrintDialog(&printer, this);
	//if (dlg->exec() != QDialog::Accepted)
	//    return;

	//document->print(&printer);

	//statusBar()->showMessage(tr("Ready"), 2000);
}

void multiview::save()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Choose a file name"), ".",
		tr("HTML (*.html *.htm)"));
	if (fileName.isEmpty())
		return;
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Dock Widgets"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}

	//QTextStream out(&file);
	//QApplication::setOverrideCursor(Qt::WaitCursor);
	//out << textEdit->toHtml();
	//QApplication::restoreOverrideCursor();

	statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
}

void multiview::undo()
{
	//QTextDocument *document = textEdit->document();
	//document->undo();
}

void multiview::setNewImage()
{
	if(mainview && preViewer)
	{
		QModelIndex index;
		preViewer->GetCurrentIndex(&index);

		if(index.row() >= 0)
		{
			int row = index.row();
			QImage img = preViewer->imgarray[row];

			QPixmap pmap = QPixmap::fromImage(img);

			mainview->setPixmap(pmap);

		}

	}

}

void multiview::zoomIn()
{
	scaleImage(1.01);
}

void multiview::zoomOut()
{
	scaleImage(0.99);
}

void multiview::scaleImage(double factor)
{
	Q_ASSERT(mainview->pixmap());
	scaleFactor *= factor;
	mainview->scale = scaleFactor;

	mainview->resize(scaleFactor * mainview->pixmap()->size());

	//adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	//adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 4.0);
	zoomOutAct->setEnabled(scaleFactor > 0.2);
}

void multiview::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(int(factor * scrollBar->value()
		+ ((factor - 1) * scrollBar->pageStep()/2)));

}


//---------------------------------------------------------------------
//  imageview class implementation
//
//

ImgViewer::ImgViewer(QLabel *parent)
: QLabel(parent)
{

	scale = 1.0;

	setBackgroundRole(QPalette::Base);
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	//setAutoFillBackground(true);
	setScaledContents(true);

}

void ImgViewer::paintEvent(QPaintEvent * /* event */)
{
	QBrush brush;

	QPainter painter(this);
	painter.setPen(pen);
	painter.setBrush(brush);

	if(pixmap() )
	{
		const QPixmap* pix = pixmap();

		QRectF tRc, sRc;
		tRc = QRectF(QPointF(0,0), scale* pix->size());
		sRc = QRectF(QPointF(0,0), pix->size());
		painter.drawPixmap(tRc,*pix, sRc);
	}


	//  static const QPoint points[4] = {
	//       QPoint(10, 80),
	//       QPoint(20, 10),
	//       QPoint(80, 30),
	//       QPoint(90, 70)
	//   };

	//   QRect rect(10, 20, 80, 60);

	//   QPainterPath path;
	//   path.moveTo(20, 80);
	//   path.lineTo(20, 30);
	//   path.cubicTo(80, 0, 50, 50, 80, 80);

	//   int startAngle = 30 * 16;
	//   int arcLength = 120 * 16;
	//{
	//       painter.setRenderHint(QPainter::Antialiasing, true);
	//       painter.translate(+0.5, +0.5);
	//   }

	//   for (int x = 0; x < width(); x += 100) 
	//{
	//       for (int y = 0; y < height(); y += 100) 
	//	{
	//           painter.save();
	//           painter.translate(x, y);

	//		{
	//               painter.translate(50, 50);
	//               painter.rotate(60.0);
	//               painter.scale(0.6, 0.9);
	//               painter.translate(-50, -50);
	//           }


	//        painter.drawLine(rect.bottomLeft(), rect.topRight());

	//	  painter.drawPoints(points, 4);

	//	  painter.drawPolygon(points, 4);

	//	  painter.restore();
	//       }
	//   }

	painter.setPen(palette().dark().color());
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

}

//-------- class Iconlist implementation ---------------------------------
//--------------------------------------------------------------------------

IconList::IconList(QObject *parent)
: QAbstractListModel(parent)
{


}

void IconList::addPiece(const QPixmap &pixmap, const QPoint &location)
{
	int cols = pixmaps.size();

	//pixmap.setText("Hallo!");

	beginInsertColumns(QModelIndex(), cols, cols);
	pixmaps.insert(cols, pixmap);
	locations.insert(cols, location);
	insertColumn(cols);
	endInsertColumns();
}

QVariant IconList::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int sx = _isize.width();
	int sy = _isize.height();

	//int col = index.column();
	//int row = index.row();

	if (role == Qt::DecorationRole)
		return QIcon(pixmaps.value(index.row()).scaled(sx, sy,
		Qt::KeepAspectRatio, Qt::SmoothTransformation));
	else if (role == Qt::UserRole)
		return pixmaps.value(index.row());
	else if (role == Qt::UserRole + 1)
		return locations.value(index.row());

	return QVariant();
}

int IconList::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return pixmaps.size();
}

int IconList::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return 1;//pixmaps.size();

}

QMimeData *IconList::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach (QModelIndex index, indexes) {
		if (index.isValid()) {
			QPixmap pixmap = qVariantValue<QPixmap>(data(index, Qt::UserRole));
			QPoint location = data(index, Qt::UserRole+1).toPoint();
			stream << pixmap << location;
		}
	}

	mimeData->setData("image/x-puzzle-piece", encodedData);
	return mimeData;
}

//--------------------------------------------------------------------------
//-------- class ImgPreview implementation ---------------------------------

ImgPreview::ImgPreview(QWidget *parent)
:QListView(parent)
{
	iconlist = new IconList(parent);
	iconlist->_isize = QSize(100, 100);
	iconlist->_gsize = QSize(110, 110);
	iconlist->_space = 10;

	setDragEnabled(true);
	setViewMode(QListView::IconMode);
	setIconSize(iconlist->_isize);
	setGridSize(iconlist->_gsize);

	setUniformItemSizes(true);
	setFlow(QListView::LeftToRight);
	setWrapping(false);

	setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));     

	setSpacing(iconlist->_space);
	setMovement(QListView::Snap);
	setAcceptDrops(true);
	setDropIndicatorShown(true);

	setMinimumHeight(iconlist->_gsize.height());


	//setModel(iconlist);

}

//#define DATAPATH "D:\\My Works\\Programming\\imageData\\"
#define DATAPATH "D:\\SunData\\images\\"

void ImgPreview::ReadImages()
{

	if(iconlist == NULL)
		return;

	for(int i = 0; i < 20; i++)
	{
		std::stringstream os;
		//os << DATAPATH << "castle00" << i << ".jpg";
		if(i < 9)
			os << DATAPATH << "L0_0" << i + 1<< ".jpg";
		else
			os << DATAPATH << "L0_" << i + 1 << ".jpg";

		std::string fn = os.str();

		QImage readimg(fn.c_str());
		//QImage simg = readimg.scaled(iconlist->_isize.width(), iconlist->_isize.height());

		iconlist->addPiece(QPixmap::fromImage(readimg), 
			QPoint(i, 0));
		//QPoint( i * iconlist->_gsize.width() + iconlist->_space, iconlist->_space));

		imgarray[i] = readimg;

		//iconlist->imglist.insert(i, &readimg);

		//newlabel->setMinimumHeight(50);
		//newlabel->resize(0.5 * newlabel->pixmap()->size());
		//newlabel->setFrameStyle(QFrame::StyledPanel);
		//connect(imgLabel[i], SIGNAL(clicked()), this, SLOT(setActiveLabel()));

	}

	setModel(iconlist);

	update();

}

void ImgPreview::setActiveLabel()
{

	QMessageBox msgbox(QMessageBox::Information, "info", "you pressed me!");
	msgbox.exec();

	//activeLabel = qobject_cast<QLabel *>(sender());
	//if(activeLabel)
	//    activeLabel->setFrameStyle(QFrame::Box);

}

void ImgPreview::GetCurrentIndex(QModelIndex * index)
{
	*index = currentIndex();

}


//void ImgPreview::mouseDoubleClickEvent( QMouseEvent * event )
//{
//    QMessageBox msgbox(QMessageBox::Information, "info", "you pressed me!");
//    msgbox.exec();
//
//    QModelIndex index = currentIndex();
//    int row = index.row();
//
//}
