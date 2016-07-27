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

#include "PodcastModel.h"

QHash<int, QByteArray> PodcastModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[ImageRole] = "image";

    return roles;
}

QVariant PodcastModel::data(const QModelIndex &index, int role) const {
    int row = index.row();

    if (row >= 0 && row < data_.size()) {
        switch (role) {
        case TitleRole:
            return QString(data_[row].title().c_str());
        }
    }

    return QVariant();
}

void PodcastModel::addPodcast(const QString &url) {
    threads_.emplace_back(std::thread([this, &url] {
        feed::utility::xml xml;

        const auto xml_str = xml.to_string(url.toLocal8Bit().constData());
        if (xml_str) {
            auto data = feed::rss::parse_rss(xml_str.value());
            if (data)
                emit insertRow(data.value());
            else
                emit addFailure("fucker");
        } else {
            emit addFailure("fucker");
        }
    }));
}

void PodcastModel::onInsertRow(feed::rss::rss_data data) {
    const std::size_t data_size = data_.size();
    beginInsertRows(QModelIndex(), data_size, data_size);
    data_.emplace_back(std::move(data));
    endInsertRows();
    emit addSuccess();
}
