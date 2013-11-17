#ifndef SORTTHERECORDNAMELIKEDIR_H
#define SORTTHERECORDNAMELIKEDIR_H

#include <QWidget>

namespace Ui {
class SortTheRecordNameLikeDir;
}

class SortTheRecordNameLikeDir : public QWidget
{
    Q_OBJECT
    
public:
    explicit SortTheRecordNameLikeDir(QWidget *parent = 0);
    ~SortTheRecordNameLikeDir();
    
private:
    Ui::SortTheRecordNameLikeDir *ui;
};

#endif // SORTTHERECORDNAMELIKEDIR_H
