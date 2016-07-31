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

Podcast::Podcast(const feed::rss::rss_data &data)
    : title_(data.title().c_str()) {

    }

QHash<int, QByteArray> PodcastModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[ImageRole] = "image";

    return roles;
}

QVariant PodcastModel::data(const QModelIndex &index, int role) const {
    int row = index.row();

    if (row >= 0 && row < podcasts_.size()) {
        switch (role) {
        case TitleRole:
            return podcasts_[row].title();
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
            if (data) {
                if (data->itunes())
                    emit insertRow(Podcast(data.value()));
                else
                    emit addFailure("not podcast.");
            } else {
                emit addFailure("not feed.");
            }
        } else {
            emit addFailure("not rss.");
        }
    }));
}

void PodcastModel::onInsertRow(Podcast podcast) {
    const int podcasts_size = podcasts_.size();
    beginInsertRows(QModelIndex(), podcasts_size, podcasts_size);
    podcasts_.append(std::move(podcast));
    endInsertRows();
    emit addSuccess();
}
