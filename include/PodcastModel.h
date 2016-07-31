/****************************************************************************
**
** Copyright (C) 2016 Michael Yang
** Contact: ohmyarchlinux@gmail.com
**
** This file is part of the pod_cast.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#pragma once

#include <QAbstractListModel>
#include <QString>
#include <feed/rss_parser.h>
#include <feed/utilities.h>
#include <thread>
#include <vector>

class Podcast {
    // Q_GADGET
    // Q_PROPERTY(QString title READ title)

  public:
    Podcast() {}
    Podcast(const feed::rss::rss_data &data);
    Podcast(const Podcast &other) : title_(other.title_) {}
    Podcast(Podcast &&other) noexcept : title_(std::move(other.title_)) {}

    const QString &title() const { return title_; }

  private:
    QString title_;
};

// Q_DECLARE_METATYPE(Podcast)

class PodcastModel : public QAbstractListModel {
    Q_OBJECT

  public:
    enum PodcastRoles { TitleRole = Qt::UserRole + 1, ImageRole };

    explicit PodcastModel(QObject *parent = nullptr) {
        Q_UNUSED(parent);

        connect(this, &PodcastModel::insertRow, this,
                &PodcastModel::onInsertRow); // Block?
    }
    ~PodcastModel() {
        for (std::thread &thread : threads_)
            thread.join();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);

        return podcasts_.size();
    }

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void addPodcast(const QString &url);

    void onInsertRow(Podcast podcast);

  protected:
    QHash<int, QByteArray> roleNames() const override;

  private:
    std::vector<std::thread> threads_;
    QVector<Podcast> podcasts_;

  signals:
    void insertRow(Podcast podcast);
    void addSuccess();
    void addFailure(QString errorMessage);
};
