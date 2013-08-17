
#include "multiview.h"
#include <QtGui/QImage>
#include <QtGui/QMessageBox>

multiview::multiview(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	
	//ui.setupUi(this);
	
	mainview = new ImgViewer; 
	QLayout* layout = new QVBoxLayout();

	
	
	
	
	



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

	setWindowTitle(tr("ARLab Image Sticher"));

	resize(800, 600);
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//lineEdit_4 = new QLineEdit;
	time_count=0;
	timer = new QTimer(this);
    timer->setInterval(1000); //1000ms == 1s
    connect(timer,SIGNAL(timeout()),this,SLOT(count_time()));

	pBar=new QProgressBar(statusBar());
	statusBar()->addWidget(pBar);
	
	




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

	
	


	//osgqtView = new ViewerQT;
	////osgqtView = new CompositeViewerQT;

	//   dock->setWidget(osgqtView);
	//addDockWidget(Qt::RightDockWidgetArea, dock);
	//   viewMenu->addAction(dock->toggleViewAction());

	//   //connect(paragraphsList, SIGNAL(currentTextChanged()),
	//   //        this, SLOT(addParagraph(const QString &)));
	////InitOsg();



	//侧边栏cmd输出
	QDockWidget *cmd_dock = new QDockWidget(tr("Cmd Out window"), this);
	cmd_dock->setAllowedAreas(  Qt::BottomDockWidgetArea 
		| Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	textEdit = new QTextEdit(cmd_dock);
	cursor=new QTextCursor(textEdit->textCursor());
	cursor->movePosition(QTextCursor::Start);
	lineEdit_4 = new QLineEdit;

	

	cmd_dock->setWidget(textEdit);
	
	textEdit->setMinimumHeight(100);
	textEdit->adjustSize();
	textEdit->setReadOnly(false);

	addDockWidget (Qt::BottomDockWidgetArea, dock);

	addDockWidget(Qt::BottomDockWidgetArea, cmd_dock);

	

	viewMenu->addAction(dock->toggleViewAction());
	connect(preViewer, SIGNAL(doubleClicked(const QModelIndex &)),
		this, SLOT(setNewImage()));



} 

//-------------------slots

void multiview::newFile()
{
	//QImage image("f:/kl/DSC01814.jpg");

	//if (image.isNull()) 
	//{
	//	QMessageBox::information(this, tr("Image Viewer"),
	//		tr("Cannot load %1.").arg("f:/kl/DSC01814.jpg"));
	//	return;
	//}
	//mainview->setPixmap(QPixmap::fromImage(image));
	//
	//

	//printAct->setEnabled(true);
	////updateActions();
	//zoomInAct->setEnabled(true);
	//zoomOutAct->setEnabled(true);

	//mainview->adjustSize();

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

	  

	QStringList    fileNameList;
	QString fileName,gpsfileName,sdir; 

	//-----------------获取照片目录-----//
	
	QFileDialog* fd = new QFileDialog(this);//创建对话框
	
	//fd->resize(240,320);    //设置显示的大小
	//fd->setFilter( "Images (*.png *.tif *.jpg)"); //设置文件过滤器
	fd->setViewMode(QFileDialog::List);  //设置浏览模式，有 列表（list） 模式和 详细信息（detail）两种方式
	//fd->setMode( QFileDialog::ExistingFiles ); 
	fd->setFileMode(QFileDialog::DirectoryOnly);
	if ( fd->exec() == QDialog::Accepted )   //如果成功的执行
	{
		
		fileNameList = fd->selectedFiles();      //返回文件列表的名称
		sdir = fileNameList[0]+"/";            //取第一个文件名
		QMessageBox::information(NULL, "Title",sdir, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	else
	{
		fd->close();
		return;
	}


	//---------------GPS ----------//
	
	fd->setFilter( "GPS data (*.txt)"); //设置文件过滤器
	fd->setViewMode(QFileDialog::List);  //设置浏览模式，有 列表（list） 模式和 详细信息（detail）两种方式
	fd->setFileMode( QFileDialog::ExistingFile ); 
	//fd->setFileMode(QFileDialog::DirectoryOnly);
	if ( fd->exec() == QDialog::Accepted )   //如果成功的执行
	{
		
		fileNameList = fd->selectedFiles();      //返回文件列表的名称
		gpsfileName = fileNameList[0];            //取第一个文件名
		QMessageBox::information(NULL, "Title",gpsfileName, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	else
	{
		fd->close();
		return;
	}


	/*if(preViewer)
	{
		preViewer->ReadImages(sdir);

	}*/





	

	QDateTime start_time=QDateTime::currentDateTime();
	myprocess = new QProcess(this);

	connect(myprocess, SIGNAL(readyReadStandardOutput()),
		this, SLOT(outlog()));
	time_count=1;
	timer->start(1000);
	
	QFileInfo beltlog(sdir+"belts.log");
	//if (!beltlog)
	if(!beltlog.exists())
	{
		this->execexternal(myprocess,
			"./gpsfilter -o "+sdir+"belts.log -g "+gpsfileName+" -s "+sdir,"processing GPSFilting");
	}
	else
	{
		push_message("\n---------------\n["+run_time+"] skip filting......\n---------------\n");
		//textEdit->setText(textEdit->toPlainText()+tr("\n skip filting......\n\n"));
		//QMessageBox::information(NULL, "Existing results","skip GPSFilting", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	}

	QFileInfo stich(sdir+"stich.pto");

	if(!stich.exists())
	{
		this->execexternal(myprocess,
			"./pto_gen "+sdir+"*.jpg -o "+sdir+"stich.pto --gps -f 1","generating pto");
	}
	else
	{
		push_message("\n---------------\n["+run_time+"] skip generating pto......\n---------------\n");
		//textEdit->setText(textEdit->toPlainText()+tr("\n skip generating pto......\n\n"));

	}
	QFileInfo stich_cp(sdir+"stich_cp.pto");

	if(!stich_cp.exists())
	{
		this->execexternal(myprocess,
			"./cpfindgps001 -o "+sdir+"stich_cp.pto "+sdir+"stich.pto --gps","Finding Control Points");
	}
	else
	{
		push_message("\n---------------\n["+run_time+"] skip finding control points......\n---------------\n");
		//textEdit->setText(textEdit->toPlainText()+tr("\n skip finding control points......\n\n"));

	}

	QFileInfo stich_cp_clean(sdir+"stich_cp_clean.pto");

	if(!stich_cp_clean.exists())
	{

		this->execexternal(myprocess,
			"./cpclean -o "+sdir+"stich_cp_clean.pto "+sdir+"stich_cp.pto","Cleaning Control Points");
	}
	else
	{
		push_message("\n---------------\n["+run_time+"] skip cleaning control points......\n---------------\n");
		//textEdit->setText(textEdit->toPlainText()+tr("\n skip cleaning control points......\n\n"));
	}
	QFileInfo stich_cp_clean_line(sdir+"stich_cp_clean_linefind.pto");

	if(!stich_cp_clean_line.exists())
	{
		this->execexternal(myprocess,
			"./linefind -o "+sdir+"stich_cp_clean_linefind.pto "+sdir+"stich_cp_clean.pto","Finding vertical lines");
	}
	else
	{
		push_message("\n---------------\n["+run_time+"] skip finding vertical lines......\n---------------\n");
		//textEdit->setText(textEdit->toPlainText()+tr("\n skip finding vertical lines......\n\n"));


	}

	this->execexternal(myprocess,
		"./checkpto "+sdir+"stich_cp_clean_linefind.pto","Checking PTO");

	//----------------insert check
	QFileInfo stich_cp_clean_line_op(sdir+"stich_cp_clean_linefind_optimised.pto");

	if(!stich_cp_clean_line_op.exists())
	{
		this->execexternal(myprocess,
			"./autooptimiser -a  -l -s -o "+sdir+"stich_cp_clean_linefind_optimised.pto "+sdir+"stich_cp_clean_linefind.pto","optimising");
	}
	else
	{
		push_message("\n---------------\n["+run_time+"] skip optimising......\n---------------\n");
		//textEdit->setText(textEdit->toPlainText()+tr("\n skip optimising......\n\n"));

	}




	this->execexternal(myprocess,
		"./pano_modify --canvas=20%% --crop=AUTO -o "+sdir+"stich_cp_clean_linefind_optimised_mod.pto "+sdir+"stich_cp_clean_linefind_optimised.pto ","modifying");

	this->execexternal(myprocess,
		"./nona -g  -z LZW -r ldr -m TIFF_m -o "+sdir+"temp  "+sdir+"stich_cp_clean_linefind_optimised_mod.pto ","resampling");
	
	QDateTime end_time;
	if(this->execexternal(myprocess,
		"./enblend --compression=LZW  -o "+sdir+"final_output.tif -- "+sdir+"temp*.tif --gpu -m 12000","Enblending")==0)
	{
		end_time=QDateTime::currentDateTime();
		this->push_message("\n---------------\nSuccessfully processed in "+run_time);
		this->push_message("\nfrom "+start_time.toString("yyyy-MM-dd hh:mm:ss ddd")+" to "+end_time.toString("yyyy-MM-dd hh:mm:ss ddd"));
	};
	

	//myprocess->start("./gpsfilter -o f:/kl/qtout.txt -g f:/kl/02.txt -s f:/kl");
	//// For debugging: Wait until the process has finished.
	////myprocess->waitForFinished();
	//while (! myprocess->waitForFinished(100)) { //启动程序后，用循环等待其结束，如果对程序何时结束并不关心，以下代码可以不需要。
	//	if (myprocess->state() == QProcess::NotRunning) { //process failed
	//		if (myprocess->exitCode() != 0) { //error when run process
	//			QMessageBox::critical(NULL,"Wrong Exitcode", tr("Error exitcode"));
	//			return;
	//		}


	//	}


	//	qApp->processEvents(); //防止UI死锁，一般情况下，用这种等一小段时间（这 里是300ms），让UI响应一次的办法，已经足够使用了。


	//}	
	//if (myprocess->exitCode() != 0) { //error when run process
	//	QMessageBox::critical(NULL,"Wrong Exitcode", tr("Error exitcode"));
	//	return;
	//}







	//	char * ch=(char*)str.c_str();
	//QMessageBox::information(NULL, "Title",ch, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);


	//h.pto_gen(fileNameList,"qout.pto");
	//h.cpfind("qout.pto","cpout.pto");

	//h.readData("cpout.pto");
	//h.loadProject("cpout.pto");



	//h.cpoints=h.pano.getCtrlPoints();




	//char *  args[]={"","-oout.pto -p2","*.jpg"};//这个地方很奇怪，要多加一个空参数，而且-o后面不能有空格
	//ptogen::ptogen_main(3,args);

	//char * args2[]={"","out.pto","-o out2.pto"};
	//cpfind::cpfind_main(3,args2);
	timer->stop();







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
			QImage img = *(preViewer->imglist[row]);

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


void multiview::outlog()
{
	//static QString text;
	QString abc = myprocess->readAllStandardOutput();
	emit outlogtext(abc);
	//text+=abc;
	textEdit->setText(textEdit->toPlainText()+abc);
	cursor->movePosition(QTextCursor::End);
	textEdit->setTextCursor(*cursor);
	
	//lineEdit_4->setText(abc);
}
void multiview::tick(QString message)
{
	static int n;
	int i;
	QString messageToShow="["+run_time+"] "+message;
	for(i=0;i<n;i++)
	{
		messageToShow+=".";

	}
	n++;
	if (n>15) n=0;

	statusBar()->showMessage(messageToShow);
	

}

int multiview::execexternal(QProcess* myprocess,QString command,QString tickmessage)
{
	int eCode;
	
	myprocess->start(command);
	// For debugging: Wait until the process has finished.
	//myprocess->waitForFinished();
	push_message("\n---------------\n["+run_time+"] start "+tickmessage+"\n---------------\n");
	while (! myprocess->waitForFinished(100)) 
	{ //启动程序后，用循环等待其结束，如果对程序何时结束并不关心，以下代码可以不需要。


		tick(tickmessage);
		if (myprocess->state() == QProcess::NotRunning) 
		{ //process failed
			eCode=myprocess->exitCode() ;
			if (eCode != 0)
			{ //error when run process
				QMessageBox::critical(NULL,"Wrong Exitcode inner", tickmessage+tr(" got an error exitcode ")+QString::number(eCode,16));
				return eCode;
			}
			else
			{
				statusBar()->showMessage(tr("Ready."));
				return myprocess->exitCode();

			}


		}


		qApp->processEvents(); //防止UI死锁，一般情况下，用这种等一小段时间（这 里是300ms），让UI响应一次的办法，已经足够使用了。


	}	
	eCode=myprocess->exitCode() ;
	if (eCode != 0) { //error when run process
		QMessageBox::critical(NULL,"Wrong Exitcode outer", tickmessage+tr(" got an error exitcode ")+QString::number(eCode,16));
		return myprocess->exitCode();
	}
	push_message("\n---------------\n["+run_time+"] "+tickmessage+" finished\n---------------\n");
	return myprocess->exitCode();

}
void multiview::push_message(QString message)
{
	cursor->movePosition(QTextCursor::End);
	textEdit->setTextCursor(*cursor);

	textEdit->setText(textEdit->toPlainText()+message+"\n");

	cursor->movePosition(QTextCursor::End);
	textEdit->setTextCursor(*cursor);
}


void multiview::count_time()
{
	time_count++;
	int day,hour,minute,second;
	day=time_count/24/3600;
	hour=(time_count/3600)%24;
	minute=(time_count/60)%60;
	second=time_count%60;

	run_time.clear();

	run_time+=QString::number(day,10)+" d ";
	run_time+=QString::number(hour,10)+" h ";
	run_time+=QString::number(minute,10)+" m ";
	run_time+=QString::number(second,10)+" s ";

	

	
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


void ImgPreview::ReadImages(QString sdir)
{

	if(iconlist == NULL)
		return;
	QStringList fileList;
	QDir dir(sdir);
	if (!dir.exists()) 
		return; 
	dir.setFilter(QDir::Dirs|QDir::Files); 
	dir.setSorting(QDir::Time |QDir::Reversed);//排序方式 修改时间从小到大 
	QFileInfoList list = dir.entryInfoList(); 
	int i;
	for (i=0;i<list.size();i++)
	{
		QFileInfo fileInfo = list.at(i); 
		if (fileInfo.completeSuffix()=="jpg"||fileInfo.completeSuffix()=="JPG")
			qDebug()<<fileInfo.path()+"/"+fileInfo.fileName()<<endl;
		QString imageFile=fileInfo.path()+"/"+fileInfo.fileName();
		QImage readimg(imageFile);
		iconlist->addPiece(QPixmap::fromImage(readimg), 
			QPoint(i, 0));
		imglist.push_back(&readimg);


	}


	for( i = 0; i < 20; i++)
	{
		

		/*QImage readimg(fn.c_str());
		iconlist->addPiece(QPixmap::fromImage(readimg), 
			QPoint(i, 0));
		imglist.push_back(&readimg);*/






		//QImage simg = readimg.scaled(iconlist->_isize.width(), iconlist->_isize.height());

		
		//QPoint( i * iconlist->_gsize.width() + iconlist->_space, iconlist->_space));

		

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
