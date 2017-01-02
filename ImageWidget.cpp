#include "ImageWidget.h"
#include "ui_ImageWidget.h"

#include <QFileInfoList>
#include <QKeyEvent>
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ImageWidget),
	m_image(QImage())
{
	ui->setupUi(this);

	QPalette p = palette();
	p.setColor(QPalette::Background, Qt::black);
	setPalette(p);

	setWindowFlags(Qt::WindowStaysOnTopHint);
}

ImageWidget::~ImageWidget()
{
	delete ui;
}

void ImageWidget::setImage(const QString &filename)
{
	QFileInfo fileInfo(filename);

	if (!fileInfo.exists()) return;

	setWindowTitle(fileInfo.fileName());
	m_image = QImage(fileInfo.filePath());

	update();
}

void ImageWidget::paintEvent(QPaintEvent *)
{
	if (m_image.isNull()) return;

	QPainter painter(this);
	painter.save();

	painter.setRenderHint(QPainter::Antialiasing, true);

	QImage scaledImage(m_image.scaled(this->size(), Qt::KeepAspectRatio));

	QPoint pos(0, 0);
	pos.setX((width() - scaledImage.width()) / 2);
	pos.setY((height() - scaledImage.height()) / 2);

	painter.drawImage(pos, scaledImage);

	painter.restore();
}

void ImageWidget::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
	{
		emit escapeKeyPressed();
	}
}

void ImageWidget::closeEvent(QCloseEvent *)
{
	emit windowClosed();
}
