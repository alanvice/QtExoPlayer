import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import com.kdab.android 1.0
import Qt.labs.platform 1.0
import Qt.labs.folderlistmodel 2.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("SurfaceTexture example")

    property int currentIndex: -1

    FolderListModel {
        id: folderModel
        folder: StandardPaths.writableLocation(StandardPaths.DownloadLocation)
        showDirs: false
        showFiles: true
        nameFilters: [ "*.mp4", "*.mov", "*.mkv", "*.webm" ]
        showDotAndDotDot: false
        sortField: FolderListModel.Name
    }

    SurfaceTexture {
        id: surfaceTexture
        anchors.fill: parent

        AndroidMediaPlayer {
            id: player
            videoOut: surfaceTexture
        }
    }

    Text {
        id: info
        anchors.centerIn: parent
        color: "white"
        text: "Tap to start the player\nTap again to skip to next\ncurrent:\n"+filename
        property string filename: ""
        visible: !player.isPlaying
    }

    MouseArea {
        anchors.fill: parent
        onClicked:
            if (info.visible)
            {
                currentIndex++;
                if(currentIndex >= folderModel.count)
                    currentIndex = 0;

                if(folderModel.count > 0)
                    play(currentIndex);

                //info.visible = false;
                //player.playFile(StandardPaths.writableLocation(StandardPaths.DownloadLocation)+"/small.mp4");
                //player.playFile(StandardPaths.writableLocation(StandardPaths.DownloadLocation)+"/video.mkv");
            }
    }

    function play(index)
    {
        var file = folderModel.get(currentIndex,"filePath");
        info.filename = folderModel.get(currentIndex,"fileName");
        player.playFile(file);
    }
}
