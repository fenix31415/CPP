#include "searcher.h"
#include <deque>
#include <cstdio>
#include <QDir>
#include <QTextStream>
#include <QChar>
#include <QtDebug>

Searcher::result::result()
    : incomplete(false), gived(0)
{}

void Searcher::result::clear() {
    incomplete = true;
    lines.clear();
    gived = 0;
}

Searcher::result::result(const result& x) : incomplete(x.incomplete) {
    lines.clear();
    lines.reserve(x.lines.size());
    for(size_t i = 0; i < x.lines.size(); ++i)
        lines.push_back(x.lines[i]);
}

Searcher::Searcher() : quit(false), callback_queued(false),
    cancel(false), Str(""),
    thread([this] {
        for (;;)
        {
            std::unique_lock<std::mutex> lg(m);
            has_work_cv.wait(lg, [this] {
                return Str.length() > 0 || quit;
            });

            if (quit)
                break;

            QString fl = this->File;
            QString st = this->Str;

            current_result.clear();

            queue_callback();

            lg.unlock();
            do_search(fl, st);
            lg.lock();

            current_result.incomplete = cancel.load();
            queue_callback();

            if (!cancel.load())
                Str = "";
            cancel.store(false);
        }
    })
{}

Searcher::~Searcher()
{
    cancel.store(true);
    {
        std::unique_lock<std::mutex> lg(m);
        quit = true;
        has_work_cv.notify_all();
    }
    thread.join();
}

void Searcher::stop() {
    Str = "";
    cancel.store(true);
}

void Searcher::start_searching(const QString &F, const QString &S)
{
    std::unique_lock<std::mutex> lg(m);
    if (Str != "") {
        cancel.store(true);
    }
    File = F;
    Str = S;
    has_work_cv.notify_all();
}

void Searcher::get_result(std::vector<result::result_item> &ans) {
    std::unique_lock<std::mutex> lg(m);
    current_result.get_result(ans);
}

bool Searcher::incomplete() {
    //std::unique_lock<std::mutex> lg(m);
    return current_result.incomplete;
}

size_t Searcher::get_count() {
    //std::unique_lock<std::mutex> lg(m);
    return current_result.lines.size();
}

void Searcher::result::get_result(std::vector<result::result_item>& ans)
{
    size_t start = gived;
    size_t fin = std::min(start + MAXGIV, lines.size());
    for(size_t i = start; i < fin; ++i) {
        ans.push_back(lines[i]);
    }
    gived += (fin-start);
}

QString convertBuf(const std::deque<char>& b) {
    std::string ans = std::string(b.begin(), b.end());
    return ans.c_str();
}

bool Searcher::prefix_find(const char *F, const char *P) {
    FILE* fp = std::fopen(F, "r");
    if (!fp) {
        qInfo() << "Can't open";
        return true;
    }

    size_t count = 0;
    size_t l;
    std::vector<int> p;
    std::deque<char> b;
    b.resize(10/* + P.length()*/, '_');
    p.push_back(0);
    b.push_back(P[0]);
    b.pop_front();
    for (l = 1; P[l]; ++l) {
        size_t j = p[l - 1];
        while ((j > 0) && (P[l] != P[j])) {
            j = p[j - 1];
        }
        if (P[l] == P[j])
            ++j;
        p.push_back(j);
    }
    p.push_back(0);
    int ch, ind = 0;
    size_t curr = 0;
    // qInfo()<<F;
    while ((ch = std::fgetc(fp)) != EOF) {
        if (cancel.load()) {
            std::fclose(fp);
            return false;
        }
        if(ch == '\n' || ch == '\r' || ch < 32)
            b.push_back('_'); else
        b.push_back(ch);
        b.pop_front();
        while ((curr > 0) && (ch != P[curr])) {
            curr = p[curr - 1];
        }
        if (ch == P[curr])
            ++curr;
        if (curr == l && current_result.lines.size() < 10000) {
            {
                std::unique_lock<std::mutex> lg(m);
                current_result.lines.push_back({F, std::to_string(ind - l + 1).c_str(), convertBuf(b)});
                queue_callback();
            }
            ++count;
        }
        ++ind;
    }
    std::fclose(fp);
    return true;
}

void Searcher::findRecursion(const QString &P, const QString &path)
{
    if (cancel.load())
        return;
    QDir currentDir(path);
    const QString prefix = path + QLatin1Char('/');

    foreach (const QString &match, currentDir.entryList(QDir::Files)) {
        if (cancel.load())
            return;
        QString F = prefix+match;
        prefix_find(F.toStdString().c_str(), P.toStdString().c_str());
    }
    foreach (const QString &dir, currentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        findRecursion(P, prefix + dir);
    }
}

void Searcher::do_search(const QString &F, const QString &P) {
    QString path = QDir::cleanPath(F);
    /*if(QFile::exists(path)) {
        prefix_find(F.toStdString().c_str(), P.toStdString().c_str());
    } else
        findRecursion(P, path);*/
    findRecursion(P, path);
}

void Searcher::queue_callback()
{
    if (callback_queued)
        return;

    QMetaObject::invokeMethod(this, "callback", Qt::QueuedConnection);
}

void Searcher::callback()
{
    {
        std::unique_lock<std::mutex> lg(m);
        callback_queued = false;
    }
    emit result_changed();
}
