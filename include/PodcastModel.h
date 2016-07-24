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
#include <vector>

class PodcastModel : public QAbstractListModel {
    Q_OBJECT

  public:
    enum PodcastRoles { TitleRole = Qt::UserRole + 1, ImageRole };

    explicit PodcastModel(QObject *parent = nullptr) {}
    ~PodcastModel() {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);

        return data_.size();
    }

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    Q_INVOKABLE bool addPodcast(const QString &url);

  protected:
    QHash<int, QByteArray> roleNames() const override;

  private:
    std::vector<feed::rss::rss_data> data_;
};
