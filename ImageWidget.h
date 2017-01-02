#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

namespace Ui {
	class ImageWidget;
}

class ImageWidget : public QWidget
{
		Q_OBJECT

	public:
		explicit ImageWidget(QWidget *parent = 0);
		~ImageWidget();

	public:
		void setImage(const QString &filename);

	private:
		Ui::ImageWidget *ui;
		QImage m_image;

		// QWidget interface
	protected:
		void paintEvent(QPaintEvent *);
		void keyPressEvent(QKeyEvent *e);
		void closeEvent(QCloseEvent *);

	signals:
		void escapeKeyPressed();
		void windowClosed();
};

#endif // IMAGEWIDGET_H
