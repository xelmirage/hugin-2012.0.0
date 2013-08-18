
#include "multiview.h"
#include <QtGui/QImage>
#include <QtGui/QMessageBox>

multiview::multiview(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	
	//ui.setupUi(this);
	tick_max=30;
	previewSize=15;







	
	mainview = new ImgViewer; 
	QLayout* layout = new QVBoxLayout();




	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setAlignment(Qt::AlignCenter);
	scrollArea->setWidget(mainview);
	//scrollArea->setWidget(m_pListWidget);
	
	

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

	
	//statusBar()->addWidget(pBar);
	
	




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


	processAct = new QAction(QIcon("images/start.png"), tr("&Process"), this);
	processAct->setShortcut(tr("Ctrl+P"));
	processAct->setEnabled(false);
	connect(processAct, SIGNAL(triggered()), this, SLOT(process()));

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

editToolBar->addAction(processAct);
	//editToolBar->addAction(undoAct);
	pBar=new QProgressBar();
	pBar->setRange(0,tick_max);
	pBar->setTextVisible(false);
	pBar->setVisible(true);
	
	editToolBar->addWidget(pBar);
}

void multiview::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void multiview::createDockWindows()
{
	QDockWidget *dock = new QDockWidget(tr("Preview Window"), this);
	dock->setAllowedAreas(  Qt::BottomDockWidgetArea 
		| Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	

//创建QListWidget部件
    m_pListWidget = new QListWidget(this);
    //设置QListWidget中的单元项的图片大小
    m_pListWidget->setIconSize(QSize(3*previewSize, 2*previewSize));
    m_pListWidget->setResizeMode(QListView::Adjust);
    //设置QListWidget的显示模式
    m_pListWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中的单元项不可被拖动
    m_pListWidget->setMovement(QListView::Static);
    //设置QListWidget中的单元项的间距
    m_pListWidget->setSpacing(10);



//---------------end for preview/-------///
	dock->setWidget(m_pListWidget);
	m_pListWidget->setMaximumHeight(400);
	m_pListWidget->adjustSize();

	//dock->setWidget(preViewer);
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
	connect(m_pListWidget, SIGNAL(currentTextChanged(QString)),
		this, SLOT(setNewImage(QString)));
	//connect(preViewer, SIGNAL(doubleClicked(const QModelIndex &)),
		//this, SLOT(setNewImage()));



} 

//-------------------slots

void multiview::newFile()
{
	

	  

	QStringList    fileNameList;
	

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
		QMessageBox::information(NULL, "Image Souce:",sdir, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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
		QMessageBox::information(NULL, "GPS Data:",gpsfileName, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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
    time_count=1;
	timer->start(1000);

	QStringList fileList;
	QDir dir(sdir);
	if (!dir.exists()) 
		return; 
	dir.setFilter(QDir::Files); 
	dir.setSorting(QDir::Name);//排序方式 修改时间从小到大 
	QFileInfoList list = dir.entryInfoList(); 
	int i;
	int nIndex=0;
	for (i=0;i<list.size();i++)
	{
		QFileInfo fileInfo = list.at(i); 
		if (fileInfo.completeSuffix()=="jpg"||fileInfo.completeSuffix()=="JPG")
		{
			
			QString imageFile=fileInfo.path()+"/"+fileInfo.fileName();
			this->push_message("Adding "+imageFile);

			QPixmap objPixmap(imageFile);
			//生成QListWidgetItem对象(注意：其Icon图像进行了伸缩[96*96])---scaled函数
			QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(3*previewSize, 2*previewSize))),imageFile);
			//设置单元项的宽度和高度
			pItem->setSizeHint(QSize(3*previewSize, 2*previewSize));
			m_pListWidget->insertItem(nIndex, pItem);
			nIndex++;
			qApp->processEvents(); //防止UI死锁，一般情况下，用这种等一小段时间（这 里是300ms），让UI响应一次的办法，已经足够使用了。



		}
		tick("Adding files");

	}








	timer->stop();
	processAct->setEnabled(true);
	pBar->setValue(0);
	statusBar()->showMessage("Adding completed.");







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

void multiview::setNewImage(QString item)
{
	//QMessageBox::information(NULL, "Title",item, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	QImage img(item);

	QPixmap pmap = QPixmap::fromImage(img);
	float ratio=scrollArea->width()/scrollArea->height();
	if(ratio>(float)(pmap.width()/pmap.height()))
	{
		mainview->setPixmap(pmap.scaledToHeight(scrollArea->height()));

	}
	else
	{
		mainview->setPixmap(pmap.scaledToWidth(scrollArea->width()));

	}

	
	//mainview->setMaximumHeight(scrollArea->height());
	
	mainview->adjustSize();


	/*if(mainview && preViewer)
	{
		QModelIndex index;
		preViewer->GetCurrentIndex(&index);
		m_pListWidget
		

		if(index.row() >= 0)
		{
			int row = index.row();
			QImage img = *(preViewer->imglist[row]);

			QPixmap pmap = QPixmap::fromImage(img);

			mainview->setPixmap(pmap);

		}

	}*/

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
	if (n>tick_max) n=0;

	statusBar()->showMessage(messageToShow);
	pBar->setValue(n);
	

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

void multiview::process()
{
	 time_count=1;
	timer->start(1000);
	QDateTime start_time=QDateTime::currentDateTime();
	QDateTime end_time;
	myprocess = new QProcess(this);

	connect(myprocess, SIGNAL(readyReadStandardOutput()),
		this, SLOT(outlog()));
	
	
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

	if(textEdit->toPlainText().contains("All images are connected."))
	{

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


		if(this->execexternal(myprocess,
			"./enblend --compression=LZW  -o "+sdir+"final_output.tif -- "+sdir+"temp*.tif --gpu -m 12000","Enblending")==0)
		{
			end_time=QDateTime::currentDateTime();
			this->push_message("\n---------------\nSuccessfully processed in "+run_time);
			this->push_message("\nfrom "+start_time.toString("yyyy-MM-dd hh:mm:ss ddd")+" to "+end_time.toString("yyyy-MM-dd hh:mm:ss ddd"));
			statusBar()->showMessage("Successfully processed from"+start_time.toString("yyyy-MM-dd hh:mm:ss ddd")+" to "+end_time.toString("yyyy-MM-dd hh:mm:ss ddd"));
		};
	}
	else
	{
		end_time=QDateTime::currentDateTime();
		this->push_message("\n---------------\nNot all the images are conncted. "+run_time);
		this->push_message("\nfrom "+start_time.toString("yyyy-MM-dd hh:mm:ss ddd")+" to "+end_time.toString("yyyy-MM-dd hh:mm:ss ddd"));
		statusBar()->showMessage("Processing failed.  :(");
	}

	timer->stop();
	pBar->setValue(0);
		


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

void ImgViewer::paintEvent(QPaintEvent * event)
{
	QRect rect=event->rect();
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
