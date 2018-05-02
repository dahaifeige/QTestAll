#include "CameraWidget.h"
#include "Camera.h"
#include "ImageDef.h"
#include "qtest.h"
#include "BitArray.h"
CameraWidget::CameraWidget(QWidget *parent, Qt::WindowFlags f) :
	m_nImageWidth(0), m_nImageHeight(0),
	m_nWnWidth(0), m_nWnHeight(0),
	m_pAlgorithm(new QTest),
	m_Color(0, 0, 0),
	m_pImage(NULL),
	m_SrcImage(),
	m_Scale(100),
	m_pVScrollBar(new QScrollBar), m_pHScrollBar(new QScrollBar),
	m_nXLeftTop(0), m_nYLeftTop(0),
	m_nHSliderValue(0), m_nVSliderValue(0),
	m_pTimer(new QTimer),
	m_queue(),
	m_fFrame(0.0), m_fCount(0), m_fFps(0), m_fPreCount(0),
	m_fSpeed(0),
	m_xPos(0), m_yPos(0),
	m_xTempPos(0), m_yTempPos(0),
	m_sFormat(""),
	m_12BitValue(0),
	m_pBitArray(nullptr)
	//m_bSetFixed(false),
	//m_bSetFixed_Fit(false)
#ifdef ISP
	,m_pTest(new QTest),
	tempXValue1(NULL), tempXValue2(NULL),
	tempYValue1(NULL), tempYValue2(NULL),
	m_xValue1(NULL), m_xValue2(NULL),
	m_yValue1(NULL), m_yValue2(NULL)
#endif // ISP
{
	setMouseTracking(true);
	setCursor(Qt::CrossCursor);
	clear();
	m_nWnHeight = height();
	m_nWnWidth = width();

	m_pVScrollBar->setSingleStep(1);
	m_pHScrollBar->setSingleStep(1);

#ifdef ISP
	tempXValue1 = new QVector<double>;
	tempXValue2 = new QVector<double>;
	tempYValue1 = new QVector<double>;
	tempYValue2 = new QVector<double>;
#endif // ISP

	connect(m_pVScrollBar.get(), SIGNAL(valueChanged(int)), this, SLOT(GetVValue(int)));
	connect(m_pHScrollBar.get(), SIGNAL(valueChanged(int)), this, SLOT(GetHValue(int)));

	connect(m_pTimer.get(), SIGNAL(timeout()), this, SLOT(Fps()));//定时刷新信息DisplayImage
	m_pTimer->start(1000);

	connect(this, SIGNAL(ReadyDisplay(QPaintEvent*)), this, SLOT(paintEvent(QPaintEvent*)));
}

CameraWidget::~CameraWidget()
{
}

void CameraWidget::GetHValue(int value)
{
	m_pAlgorithm->CaculateLeftTopValue((float)m_nImageWidth, (float)m_nWnWidth, (float)m_pHScrollBar->maximum(), (float)value, &m_nXLeftTop, &m_nHSliderValue);
	//int max = m_pHScrollBar->maximum();
	//if (max == 0)
	//	return;
	//float imageSliderScale = (float)m_nImageWidth / max;
	//float displaySliderSCale = (float)m_nWnWidth / max;
	//m_nHSliderValue = value * imageSliderScale;
	//float displaySliderValue = value * displaySliderSCale;
	//m_nXLeftTop = (float)m_nHSliderValue - displaySliderValue;
}

void CameraWidget::GetVValue(int value)
{
	m_pAlgorithm->CaculateLeftTopValue((float)m_nImageHeight, (float)m_nWnHeight, (float)m_pVScrollBar->maximum(), (float)value, &m_nYLeftTop, &m_nVSliderValue);
	/*int max = m_pVScrollBar->maximum();
	if (max == 0)
		return;
	float imageSliderScale = (float)m_nImageHeight / max;
	float displaySliderSCale = (float)m_nWnHeight / max;
	m_nVSliderValue = value * imageSliderScale;
	float displaySliderValue = value * displaySliderSCale;
	m_nYLeftTop = (float)m_nVSliderValue - displaySliderValue;*/
}

void CameraWidget::mouseMoveEvent(QMouseEvent *event)
{
	m_xTempPos = event->pos().x();
	m_yTempPos = event->pos().y();
	if ( (!m_pBitArray->GetItemStatus(Status_ZoomIn)) && (!m_pBitArray->GetItemStatus(Status_ZoomOut)) )
	{
		m_xPos = event->pos().x();
		m_yPos = event->pos().y();
	}
}

void CameraWidget::CallBack(std::shared_ptr<gcp2::Image> image)
{	
	if (!image || !image->GetImageInfo().width)
		return;
	m_fCount++;
	m_nImageWidth = image->GetImageInfo().width;
	m_nImageHeight = image->GetImageInfo().height;
	//call many times. constructor and destructor cost much. you may directly call  std::mutex
	std::lock_guard<std::mutex> l(m_Mutex);
	m_pImage = image;
	update();
	/*QPaintEvent *event = new QPaintEvent(QRect());
	emit ReadyDisplay(event);*/
#ifdef ISP
	uchar* buff = image->GetImageBuff();
	uchar* needBuff = new uchar[60 + 1];
	for (int i = 0; i < 60; i++)
	{
		needBuff[i] = buff[image->GetImageInfo().width*20 - 60 + i];
	}


	QBitArray array;
	int size = _msize(needBuff) / sizeof(needBuff[0]) - 1;
	m_pTest->uchar2QBitArray(needBuff, size, &array);

	QBitArray array_1(32);
	int index_1 = 307;
	for (int i = 0; i < 32; i++)
	{
		array_1[i] = array[index_1];
		index_1++;
	}
	uint value_1 = 0;
	m_pTest->bitArray2Int(&array_1, &value_1);

	QBitArray array_2(32);
	int index_2 = 339;
	for (int i = 0; i < 32; i++)
	{
		array_2[i] = array[index_2];
		index_2++;
	}
	uint value_2 = 0;
	m_pTest->bitArray2Int(&array_2, &value_2);

	tempXValue1->append(m_fCount);
	tempYValue1->append(value_1);

	tempXValue2->append(m_fCount);
	tempYValue2->append(value_2);

	m_xValue1 = std::make_shared<QVector<double>>(*tempXValue1);
	m_xValue2 = std::make_shared<QVector<double>>(*tempXValue2);

	m_yValue1 = std::make_shared<QVector<double>>(*tempYValue1);
	m_yValue2 = std::make_shared<QVector<double>>(*tempYValue2);
#endif
}

void CameraWidget::paintEvent(QPaintEvent *event)
{
	std::lock_guard<std::mutex> l(m_Mutex);
	if (!m_pImage || !m_pImage->GetImageInfo().width)
		return;
	switch (m_pImage->GetImageInfo().format)
	{
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO8: {
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_Grayscale8);
		m_SrcImage = std::move(tempImage);
		m_sFormat = "MONO8";
		m_Color = m_SrcImage.pixel(m_xPos, m_yPos);
		break;
	}
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO10: {
		m_pImage = m_pImage->Unpack2Image8();
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_Grayscale8);
		m_SrcImage = std::move(tempImage);
		m_sFormat = "MONO10";
		break;
	}
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO10_PACKED: {
		m_sFormat = "MONO10_PACKED";
		m_pImage = m_pImage->Unpack2Image8();
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_Grayscale8);
		m_SrcImage = std::move(tempImage);
		break;
	}
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO12: {
		m_sFormat = "MONO12";
		m_pImage = m_pImage->Unpack2Image8();
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_Grayscale8);
		m_SrcImage = std::move(tempImage);
		m_pAlgorithm->Get12BitPixelValue(m_pImage->GetImageBuff(), m_xPos, m_yPos, m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, &m_12BitValue);
		break;
	}
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO12_PACKED: {
		m_sFormat = "MONO12_PACKED";
		m_pImage = m_pImage->Unpack2Image8();
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_Grayscale8);
		m_SrcImage = std::move(tempImage);
		break;
	}
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_RGB8: {
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_RGB888);
		m_SrcImage = std::move(tempImage);
		m_sFormat = "RGB8";
		m_Color = m_SrcImage.pixel(m_xPos, m_yPos);
		break;
	}
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_BGR8: {
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_RGB888);
		m_SrcImage = std::move(tempImage);
		m_sFormat = "BGR8";
		m_Color = m_SrcImage.pixel(m_xPos, m_yPos);
		break;
	}
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_BAYER_GR8: {
		m_pImage = m_pImage->Unpack2Image8();
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_RGB888);
		m_SrcImage = std::move(tempImage);
		m_sFormat = "BAYER_GR8";
		break;
	}
	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_BAYER_RG8: {
		m_pImage = m_pImage->Unpack2Image8();
		QImage tempImage(m_pImage->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_RGB888);
		m_SrcImage = std::move(tempImage);
		m_sFormat = "BAYER_RG8";
		break;
	}
	default:
		//QMessageBox::information(this, "ERROR", "Format error!!!");
		break;
	}
	QPainter painter(this);
	if (m_pBitArray->GetItemStatus(Status_ZoomIn) || m_pBitArray->GetItemStatus(Status_ZoomOut))
	{
		QRect roiRect;
		m_pAlgorithm->ResizePrepare(m_Scale, QSize(m_nWnWidth, m_nWnHeight), QSize(m_SrcImage.width(), m_SrcImage.height()), m_nHSliderValue, m_nVSliderValue, &roiRect);
		m_pAlgorithm->GetSrcPoint(QRect(m_nXLeftTop, m_nYLeftTop, m_nWnWidth, m_nWnHeight), roiRect, QSize(m_SrcImage.width(), m_SrcImage.height()), m_xTempPos, m_yTempPos, &m_xPos, &m_yPos);
		if (m_Scale >= 100) {
			setFixedSize(m_SrcImage.width(), m_SrcImage.height());
			QRect dstRect(m_nXLeftTop, m_nYLeftTop, m_nWnWidth, m_nWnHeight);
			painter.drawImage(QRectF(dstRect), m_SrcImage, QRectF(roiRect));
		}
		else
		{
			QRect dstRect(0, 0, roiRect.width(), roiRect.height());
			QRect rect(0, 0, m_SrcImage.width(), m_SrcImage.height());
			painter.drawImage(QRectF(dstRect), m_SrcImage, QRectF(rect));
		}
	}
	else if (m_pBitArray->GetItemStatus(Status_ZoomToFit))
	{
		setFixedSize(m_nWnWidth, m_nWnHeight);
		QImage image = m_SrcImage.scaled(m_nWnWidth, m_nWnHeight);
		painter.drawImage(0, 0, image);
	}
	else
	{
		m_Scale = 100;
		setFixedSize(m_SrcImage.width(), m_SrcImage.height());
		painter.drawImage(0, 0, m_SrcImage);
	}
	if (m_pBitArray->GetItemStatus(Status_Save) || (m_pBitArray->GetItemStatus(Status_Record) && !m_pBitArray->GetItemStatus(Status_Pause)))
		SaveImage(m_SrcImage);
}
//计算帧频
void CameraWidget::Fps()
{
	m_fFrame = m_fCount - m_fPreCount;
	m_fPreCount = m_fCount;
	if (m_queue.size()< 5)
		m_queue.push_back(m_fFrame);
	else
	{
		double sumFrame = 0;
		for (auto& i : m_queue)
			sumFrame += i;
		m_fFps = sumFrame / m_queue.size();
		m_fSpeed = m_nImageHeight * m_nImageWidth * m_fFps / (1024 * 1024);
		m_queue.pop_front();
		m_queue.push_back(m_fFrame);
	}
}
//保存图片 PNG格式
void CameraWidget::SaveImage(QImage image)
{
	if (image.isNull()) {
		//	QMessageBox::information(this, "ERROR", "Image is Empty!!! ");
		return;
	}
	QDateTime dateTime;
	QString timeStr = dateTime.currentDateTime().toString("yyyyMMddHHmmss");
	QFile data("./Image/" + timeStr + ".BMP");
	if (!data.open(QIODevice::ReadWrite))
		QMessageBox::information(this, "ERROR", "save file failed! " + data.errorString());
	QByteArray b;
	QBuffer buffer(&b);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "BMP");
	data.write(b);
	data.close();
	if (m_pBitArray->GetItemStatus(Status_Save)) m_pBitArray->SetItemStatus(Status_Save, false);
}

//QPixmap pixmap;
//pixmap = pixmap.fromImage(m_SrcImage);
//pixmap = pixmap.scaled(QSize(m_SrcImage.width(), m_SrcImage.height()));//适应窗口大小
//painter.drawPixmap(dstRect, pixmap, roiRect);

//QPixmap pixmap;
//pixmap = pixmap.fromImage(m_SrcImage);
//pixmap = pixmap.scaled(QSize(m_SrcImage.width(), m_SrcImage.height()));//适应窗口大小
//painter.drawPixmap(dstRect, pixmap, rect);

/*m_SrcImage.scaled(size());*/
//QPixmap pixmap;
//pixmap = pixmap.fromImage(m_SrcImage, Qt::NoFormatConversion);
//pixmap = pixmap.scaled(QSize(m_nWnWidth, m_nWnHeight));//适应窗口大小
//painter.drawPixmap(0, 0, pixmap);

//QPixmap pixmap;
//pixmap = pixmap.fromImage(m_SrcImage, Qt::NoFormatConversion);
////pixmap = pixmap.scaled(QSize(m_SrcImage.width(), m_SrcImage.height()));//适应窗口大小
//painter.drawPixmap(0, 0, pixmap);

//void CameraWidget::mousePressEvent(QMouseEvent *event)
//{
//	if (event->button() == Qt::LeftButton)
//	{
//		m_bMousePress = true;
//		m_bResize = false;
//		m_beginPoint = event->pos();
//	}
//	return QWidget::mousePressEvent(event);
//}
//
//void CameraWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//	m_endPoint = event->pos();
//	if (m_bMouseMove && m_bMousePress)
//		m_bResize = true;
//	m_bMouseMove = false;
//	m_bMousePress = false;
//	return QWidget::mouseReleaseEvent(event);
//}
//void CameraWidget::detectAndDraw(Mat& img,CascadeClassifier& cascade, CascadeClassifier& nestedCascade,double scale)
//{
//	using cv::resize;
//
//	int i = 0;
//	double t = 0;
//	vector<Rect> faces;
//	const static Scalar colors[] = { CV_RGB(0,0,255),
//			CV_RGB(0,128,255),
//			CV_RGB(0,255,255),
//			CV_RGB(0,255,0),
//			CV_RGB(255,128,0),
//			CV_RGB(255,255,0),
//			CV_RGB(255,0,0),
//			CV_RGB(255,0,255) };//用不同的颜色表示不同的人脸
//
//	Mat gray, smallImg(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);//将图片缩小，加快检测速度
//
////	cvtColor(img, gray, CV_BGR2GRAY);//因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像
//	resize(img, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);//将尺寸缩小到1/scale,用线性插值
//	equalizeHist(smallImg, smallImg);//直方图均衡
//	t = (double)cvGetTickCount();//用来计算算法执行时间
//	cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));	//|CV_HAAR_FIND_BIGGEST_OBJECT//|CV_HAAR_DO_ROUGH_SEARCH
//	t = (double)cvGetTickCount() - t;//相减为算法执行的时间
//	printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
//	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
//	{
//		Mat smallImgROI;
//		vector<Rect> nestedObjects;
//		Point center;
//		Scalar color = colors[i % 8];
//		int radius;
//		center.x = cvRound((r->x + r->width*0.5)*scale);//还原成原来的大小
//		center.y = cvRound((r->y + r->height*0.5)*scale);
//		radius = cvRound((r->width + r->height)*0.25*scale);
//		circle(img, center, radius, color, 3, 8, 0);
//
//		//检测人眼，在每幅人脸图上画出人眼
//		if (nestedCascade.empty())
//			continue;
//		smallImgROI = smallImg(*r);
//
//		//和上面的函数功能一样
//		nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));//|CV_HAAR_FIND_BIGGEST_OBJECT//|CV_HAAR_DO_ROUGH_SEARCH//|CV_HAAR_DO_CANNY_PRUNING
//		for (vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++)
//		{
//			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
//			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
//			radius = cvRound((nr->width + nr->height)*0.25*scale);
//			circle(img, center, radius, color, 3, 8, 0);//将眼睛也画出来，和对应人脸的图形是一样的
//		}
//	}
//	imshow("result", img);
//}

//void CameraWidget::ResizePrepare(IN const float scale, IN const qreal hSliderValue, IN const qreal vSliderValue, IN const int imageWidth, IN const int imageHeight, OUT QRect * roiRect)
//{
//	float roiWidth = 0;
//	float roiHeight = 0;
//	if (scale >= 100)
//	{
//		roiWidth = m_nWnWidth * 100 / scale;
//		roiHeight = m_nWnHeight * 100 / scale;
//	}
//	else
//	{
//		roiWidth = imageWidth * scale / 100;
//		roiHeight = imageHeight * scale / 100;
//	}
//
//	qreal roiXPos = 0;
//	qreal roiYPos = 0;
//
//	if (hSliderValue > imageWidth - roiWidth)
//		roiXPos = imageWidth - roiWidth;
//	else
//		roiXPos = hSliderValue;
//
//	if (vSliderValue > imageHeight - roiHeight)
//		roiYPos = imageHeight - roiHeight;
//	else
//		roiYPos = vSliderValue;
//
//	roiRect->setX(roiXPos);
//	roiRect->setY(roiYPos);
//	roiRect->setWidth(roiWidth);
//	roiRect->setHeight(roiHeight);
//
//}
//
//void CameraWidget::GetSrcPoint(IN const int xPos, IN const int yPos, IN const qreal roiX, IN const qreal roiY, IN const qreal xLeftTop, IN const qreal yLeftTop, IN const qreal roiWidth, IN const qreal roiHeight, OUT int *srcXPos, OUT int *srcYPos)
//{
//	
//	// (ypos - yLeftTop) / m_nWnHeight = (srcYPos - roiY) / roiHeight
//	if (m_nWnWidth == 0)
//		return;
//	if (xPos == m_nImageWidth - 1)
//		*srcXPos = xPos;
//	else 
//	{
//		float tempX = (float)roiWidth / m_nWnWidth;
//		float tempXX = tempX * (xPos - xLeftTop);
//		*srcXPos = (int)(tempXX + roiX);
//	}
//	
//	if (m_nWnHeight == 0)
//		return;
//
//	if (yPos == m_nImageHeight - 1)
//		*srcXPos = yPos;
//	else
//	{
//		float tempY = (float)roiHeight / m_nWnHeight;
//		float tempYY = tempY * (yPos - yLeftTop);
//		*srcYPos = (int)(tempYY + roiY);
//	}
//
//}


//void CameraWidget::DisplayImage()
//{
//	if (!m_pImage)
//		return;
//	if (!m_bReadyDisplay)
//		return;
//	std::shared_ptr<gcp2::Image> image = m_pImage;
//
//	QImage temImage(image->GetImageBuff(), m_pImage->GetImageInfo().width, m_pImage->GetImageInfo().height, QImage::Format_Grayscale8);//image->GetImageBuff(),
//	//uchar* ch = temImage.bits();
//	//uchar* buff = image->GetImageBuff();
//	//for (int i = 0;i < image->GetImageInfo().width * image->GetImageInfo().height;i++)
//	//{
//	//	*ch = *buff;
//	//	ch++;
//	//	buff++;
//	//}
//
//	QByteArray ba;
//	QBuffer buffer(&ba);
//	buffer.open(QIODevice::WriteOnly);
//	temImage.save(&buffer, "PNG"); // writes image into ba in PNG format
//	m_Image = temImage;
//	if (m_bZoom || (m_bZoomRect && m_bResize))
//	{
//		gcp::IMG_INFO dstInfo;
//		gcp2::IMAGE_INFO srcInfo = m_pImage->GetImageInfo();
//		uchar * srcBuff = m_pImage->GetImageBuff();
//
//		Zoom(&srcInfo, srcBuff, &dstInfo, &m_ZoomImage);
//		m_Pixmap = m_Pixmap.fromImage(m_ZoomImage, Qt::NoFormatConversion);
//		m_Pixmap = m_Pixmap.scaled(m_ZoomImage.size());
//	}
//	else
//	{
//		m_Pixmap = m_Pixmap.fromImage(temImage, Qt::NoFormatConversion);
//		m_Pixmap = m_Pixmap.scaled(temImage.size());
//	}
//	
//	//m_Palette.setBrush(backgroundRole(), QBrush(m_Pixmap));
//	//setPixmap(m_Pixmap);
//	//setPalette(m_Palette);
//	m_bReadyDisplay = false;
//	//update();
//}
//

//srcImgInfo->width 2592
//bool okk = temImage.loadFromData(image->GetImageBuff(), image->GetImageInfo().width * image->GetImageInfo().height);
//uchar* ch = temImage.bits();
//uchar* buff = image->GetImageBuff();
//for (int i = 0;i < image->GetImageInfo().width * image->GetImageInfo().height;i++)
//{
//	*ch = *buff;
//	ch++;
//	buff++;
//}
//void CameraWidget::paintEvent(QPaintEvent *event)
//{
//	//Display image
//	//if (Zoom)  Get Point set scale > 100 (value = value + value*10%, max = 2000) to get image;
//	//			 set scale < 100 (value = value - value*10% , min > 0)to get Image;
//	//			 (Zoom100%)set scale = 100 (value = 100 % falut value)to get Image;
//	//else if (ZoomRect && m_resize) Get Point and set scale = 200 % to get Image;
//	//else if (ZoomTofit) set CameraWidget(1000 * 900) set scale = value to get Image;
//	//else  to get Image;
//	QPainter painter(this);
//	if (m_bPause)
//	{
//		QImage image;
//		image.load("./icon/pause18.png");
//		painter.drawImage(200, 150, image);
//		return;
//	}
//	std::shared_ptr<gcp2::Image> pImage = m_pImage;
//	if (!pImage)
//		return;
//
//	QImage temImage;
//	switch (pImage->GetImageInfo().format)
//	{
//	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO8: {
//		QImage image(pImage->GetImageBuff(), pImage->GetImageInfo().width, pImage->GetImageInfo().height, QImage::Format_Grayscale8);
//		temImage = image;
//		break;
//	}
//	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO10: {
//		QImage image(pImage->GetImageBuff(), pImage->GetImageInfo().width, pImage->GetImageInfo().height, QImage::Format_Grayscale8);
//		temImage = image;
//		break;
//	}
//	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO10_PACKED: {
//		QImage image(pImage->GetImageBuff(), pImage->GetImageInfo().width, pImage->GetImageInfo().height, QImage::Format_Grayscale8);
//		temImage = image;
//		break;
//	}
//	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO12: {
//		QImage image(pImage->GetImageBuff(), pImage->GetImageInfo().width, pImage->GetImageInfo().height, QImage::Format_Grayscale8);
//		temImage = image;
//		break;
//	}
//	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_MONO12_PACKED: {
//		QImage image(pImage->GetImageBuff(), pImage->GetImageInfo().width, pImage->GetImageInfo().height, QImage::Format_Grayscale8);
//		temImage = image;
//		break;
//	}
//	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_RGB8: {
//		QImage image(pImage->GetImageBuff(), pImage->GetImageInfo().width, pImage->GetImageInfo().height, QImage::Format_RGB888);
//		temImage = image;
//		break;
//	}
//	case gcp2::PIXEL_FORMAT::PIXEL_FORMAT_BGR8: {
//		QImage image(pImage->GetImageBuff(), pImage->GetImageInfo().width, pImage->GetImageInfo().height, QImage::Format_RGB888);
//		temImage = image;
//		break;
//	}
//	default:
//		break;
//
//	}
//	//	QImage temImage(pImage->GetImageBuff(), pImage->GetImageInfo().width, pImage->GetImageInfo().height, QImage::Format_Grayscale8);
//	if (m_bZoom)
//	{
//		gcp::IMG_INFO dstInfo;
//		gcp2::IMAGE_INFO srcInfo = pImage->GetImageInfo();
//		uchar * srcBuff = pImage->GetImageBuff();
//		gcp::IMG_POINT point = { ((gcp::IMG_UINT)m_nVValue,(gcp::IMG_UINT)m_nVValue) };
//		double tgtx, tgty;
//		Zoom(&srcInfo, srcBuff, &point, m_Scale, &dstInfo, &m_ZoomImage, &tgtx, &tgty);
//		setFixedSize(dstInfo.width, dstInfo.height);// srcImgInfo->width 2592 srcImgInfo->height 1944
//		painter.drawImage(0, 0, m_ZoomImage);
//		if (m_bZoomRect && m_bResize)
//			drawZoomRect(painter, temImage, point, tgtx, tgty);
//		if (m_bSave || m_bRecord)
//			SaveImage(m_ZoomImage);
//	}
//	else if (m_bZoomToFit)
//	{
//		setFixedSize(1000, 750);
//		QPixmap pixmap;
//		pixmap = pixmap.fromImage(temImage, Qt::NoFormatConversion);
//		pixmap = pixmap.scaled(size());
//		painter.drawPixmap(0, 0, pixmap);
//		if (m_bSave || m_bRecord)
//			SaveImage(pixmap.toImage());
//	}
//	else
//	{
//		setFixedSize(temImage.width(), temImage.height());// srcImgInfo->width 2592 srcImgInfo->height 1944
//		painter.drawImage(0, 0, temImage);
//		if (m_bZoomRect && m_bResize)
//			drawZoomRect(painter, temImage);
//		if (m_bSave || m_bRecord)
//			SaveImage(temImage);
//	}
//	if (m_bMousePress)
//	{
//		QPen pen = QPen();
//		pen.setColor(Qt::red);
//		pen.setStyle(Qt::SolidLine);
//		pen.setWidth(1);
//		painter.setPen(pen);
//		painter.drawRect(QRect(m_beginPoint, m_endPoint));
//	}
//}

//painter.drawImage(0, 0, temImage);
//float width = 1000 * m_Scale / 100;
//float height = 760 * m_Scale / 100;
//QRect srcRect(m_nHValue, m_nVValue, 1000, 760);
//float x = width + m_nHValue;
//float y = height + m_nVValue;
//if (width > temImage.width() || height > temImage.height())// (int)width, (int)height
//{
//	setFixedSize(width, height);// srcImgInfo->width 2592 srcImgInfo->height 1944
//	QRect dstRect(0, 0, (int)width, (int)height);
//	QPixmap pixmap;
//	pixmap = pixmap.fromImage(temImage);
//	painter.drawPixmap(dstRect, pixmap, srcRect);
//}
//else if (x <= temImage.width() && y <= temImage.height())// (int)width, (int)height
//{
//	QRect dstRect(m_nHValue, m_nVValue, (int)width, (int)height);
//	QPixmap pixmap;
//	pixmap = pixmap.fromImage(temImage);
//	painter.drawPixmap(dstRect, pixmap, srcRect);
//}
//else if(x > temImage.width() && y <= temImage.height())
//{
//	QRect dstRect((temImage.width() - width), m_nVValue, (int)width, (int)height);
//	QPixmap pixmap;
//	pixmap = pixmap.fromImage(temImage);
//	painter.drawPixmap(dstRect, pixmap, srcRect);
//}
//else if (x <= temImage.width() && y > temImage.height())
//{
//	QRect dstRect(m_nHValue, (temImage.height() - height), (int)width, (int)height);
//	QPixmap pixmap;
//	pixmap = pixmap.fromImage(temImage);
//	painter.drawPixmap(dstRect, pixmap, srcRect);
//}
//else if (x > temImage.width() && y > temImage.height())
//{
//	QRect dstRect(temImage.width() - width, (temImage.height() - height), (int)width, (int)height);
//	QPixmap pixmap;
//	pixmap = pixmap.fromImage(temImage);
//	painter.drawPixmap(dstRect, pixmap, srcRect);
//}

//QPixmap pix;
//pix = pix.fromImage(temImage);
//qreal width = pix.width(); //获得以前图片的宽和高
//qreal height = pix.height();
//pix = pix.scaled(width * m_Scale , height * 2, Qt::KeepAspectRatio);
//setFixedSize(pix.width(), pix.height());
//painter.drawPixmap(0, 0, pix);