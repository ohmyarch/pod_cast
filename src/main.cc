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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#if defined(Q_OS_UNIX) || defined(Q_OS_WIN)
#include <QMenu>
#include <QSystemTrayIcon>
#include <memory>
#endif

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

#if defined(Q_OS_WIN)
    QQuickStyle::setStyle("Universal");
#else
    QQuickStyle::setStyle("Material");
#endif

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

#if defined(Q_OS_UNIX) || defined(Q_OS_WIN)
    QObject *root = engine.rootObjects()[0];

    QAction *quit_action = new QAction(QObject::tr("Quit"), root);

    std::unique_ptr<QMenu> tray_icon_menu(new QMenu());
    tray_icon_menu->addAction(quit_action);

    QObject::connect(quit_action, &QAction::triggered, &QCoreApplication::quit);

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(
        QIcon(QStringLiteral(":/images/pod_cast.png")), root);
    trayIcon->setContextMenu(tray_icon_menu.get());
    trayIcon->show();
#endif

    return app.exec();
}
