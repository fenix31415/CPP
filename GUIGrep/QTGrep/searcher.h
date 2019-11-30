#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <deque>

class Searcher : public QObject
{
    Q_OBJECT

public:
    struct result
    {
        struct result_item {
            QString file_name;
            QString pos;
            QString buf;
        };

        result();
        result(result const& x);
        void clear();
        void get_result(std::vector<result::result_item>& ans);

        std::vector<result_item> lines;
        bool incomplete;
        size_t gived;
        static const size_t MAXGIV = 10;


    };

    Searcher();

    ~Searcher();

    void start_searching(const QString &F, const QString &S);
    void get_result(std::vector<result::result_item>& ans);
    void stop();

    bool incomplete();
    int how_gived();

signals:
    void result_changed();

public slots:
    void callback();

private:

    void do_search(const QString &F, const QString &P);
    bool prefix_find(const char *F, const char *P);
    void findRecursion(const QString &P, const QString &path);
    void queue_callback();

    std::mutex m;
    QString File;
    QString Str;
    bool quit;
    bool callback_queued;
    std::atomic<bool> cancel {false};
    std::condition_variable has_work_cv;
    result current_result;
    std::thread thread;
};

#endif // SEARCHER_H
