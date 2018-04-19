/*
    Copyright 2016, Mitch Curtis

    This file is part of Slate.

    Slate is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Slate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Slate. If not, see <http://www.gnu.org/licenses/>.
*/

#include "testhelper.h"

#include <QPainter>

#include "imagelayer.h"
#include "projectmanager.h"

TestHelper::TestHelper(int &argc, char **argv) :
    app(argc, argv, QStringLiteral("Slate Test Suite")),
    window(qobject_cast<QQuickWindow*>(app.qmlEngine()->rootObjects().first())),
    overlay(nullptr),
    projectManager(nullptr),
    imageProject(nullptr),
    tilesetProject(nullptr),
    layeredImageProject(nullptr),
    imageCanvas(nullptr),
    tileCanvas(nullptr),
    canvasSizeButton(nullptr),
    imageSizeButton(nullptr),
    modeToolButton(nullptr),
    penToolButton(nullptr),
    eyeDropperToolButton(nullptr),
    fillToolButton(nullptr),
    eraserToolButton(nullptr),
    selectionToolButton(nullptr),
    toolSizeButton(nullptr),
    undoButton(nullptr),
    redoButton(nullptr),
    penForegroundColourButton(nullptr),
    penBackgroundColourButton(nullptr),
    tilesetSwatch(nullptr),
    tilesetSwatchFlickable(nullptr),
    newLayerButton(nullptr),
    moveLayerDownButton(nullptr),
    moveLayerUpButton(nullptr),
    animationPlayPauseButton(nullptr),
    tilesetBasename("test-tileset.png")
{
}

TestHelper::~TestHelper()
{
}

void TestHelper::initTestCase()
{
    // Tests could have failed on the last run, so just enforce the default settings.
    app.settings()->setGridVisible(app.settings()->defaultGridVisible());
    QVERIFY(app.settings()->isGridVisible());
    app.settings()->setRulersVisible(false);
    QVERIFY(!app.settings()->areRulersVisible());

    // However, this should never change.
    QVERIFY(!app.settings()->loadLastOnStartup());

    QVERIFY(window->property("overlay").isValid());
    overlay = window->property("overlay").value<QQuickItem*>();
    QVERIFY(overlay);

    projectManager = app.projectManager();

    // Whenever the project manager's project changes, it means we should
    // reset our errorOccurred spy, as createNewProject() is about to be called on it.
    connect(projectManager.data(), &ProjectManager::temporaryProjectChanged, this, &TestHelper::resetCreationErrorSpy);
    // The project would have already been created by now, as a layered image project is always
    // loaded by default, so we have to manually create the spy now.
    resetCreationErrorSpy();

    layeredImageCanvas = window->findChild<LayeredImageCanvas*>("layeredImageCanvas");
    QVERIFY(layeredImageCanvas);

    project = projectManager->project();
    QVERIFY(project);

    layeredImageProject = qobject_cast<LayeredImageProject*>(project.data());
    QVERIFY(layeredImageProject);

    canvasSizeButton = window->findChild<QQuickItem*>("canvasSizeButton");
    QVERIFY(canvasSizeButton);

    imageSizeButton = window->findChild<QQuickItem*>("imageSizeButton");
    QVERIFY(imageSizeButton);

    modeToolButton = window->findChild<QQuickItem*>("modeToolButton");
    QVERIFY(modeToolButton);

    penToolButton = window->findChild<QQuickItem*>("penToolButton");
    QVERIFY(penToolButton);

    eyeDropperToolButton = window->findChild<QQuickItem*>("eyeDropperToolButton");
    QVERIFY(eyeDropperToolButton);

    fillToolButton = window->findChild<QQuickItem*>("fillToolButton");
    QVERIFY(fillToolButton);

    eraserToolButton = window->findChild<QQuickItem*>("eraserToolButton");
    QVERIFY(eraserToolButton);

    selectionToolButton = window->findChild<QQuickItem*>("selectionToolButton");
    QVERIFY(selectionToolButton);

    toolSizeButton = window->findChild<QQuickItem*>("toolSizeButton");
    QVERIFY(toolSizeButton);

    undoButton = window->findChild<QQuickItem*>("undoButton");
    QVERIFY(undoButton);

    redoButton = window->findChild<QQuickItem*>("redoButton");
    QVERIFY(redoButton);

    penForegroundColourButton = window->findChild<QQuickItem*>("penForegroundColourButton");
    QVERIFY(penForegroundColourButton);

    penBackgroundColourButton = window->findChild<QQuickItem*>("penBackgroundColourButton");
    QVERIFY(penBackgroundColourButton);

    lighterButton = window->findChild<QQuickItem*>("lighterButton");
    QVERIFY(lighterButton);

    darkerButton = window->findChild<QQuickItem*>("darkerButton");
    QVERIFY(darkerButton);

    saturateButton = window->findChild<QQuickItem*>("saturateButton");
    QVERIFY(saturateButton);

    desaturateButton = window->findChild<QQuickItem*>("desaturateButton");
    QVERIFY(desaturateButton);

    QPixmap checkerPixmap = QPixmap(":/images/checker.png");
    QCOMPARE(checkerPixmap.isNull(), false);
    mCheckerImage = checkerPixmap.toImage();
}

void TestHelper::cleanup()
{
    failureMessage.clear();

    app.settings()->clearRecentFiles();
}

void TestHelper::resetCreationErrorSpy()
{
    if (projectManager->temporaryProject())
        creationErrorOccurredSpy.reset(new QSignalSpy(projectManager->temporaryProject(), SIGNAL(errorOccurred(QString))));
}

void TestHelper::mouseEventOnCentre(QQuickItem *item, TestMouseEventType eventType)
{
    QQuickWindow *itemWindow = item->window();
    Q_ASSERT(itemWindow);

    const QPoint centre = item->mapToScene(QPointF(item->width() / 2, item->height() / 2)).toPoint();
    switch (eventType) {
    case MousePress:
        QTest::mousePress(itemWindow, Qt::LeftButton, Qt::NoModifier, centre);
        break;
    case MouseRelease:
        QTest::mouseRelease(itemWindow, Qt::LeftButton, Qt::NoModifier, centre);
        break;
    case MouseClick:
        QTest::mouseClick(itemWindow, Qt::LeftButton, Qt::NoModifier, centre);
        break;
    case MouseDoubleClick:
        QTest::mouseDClick(itemWindow, Qt::LeftButton, Qt::NoModifier, centre);
        break;
    }
}

void TestHelper::mouseEvent(QQuickItem *item, const QPointF &localPos, TestMouseEventType eventType, Qt::KeyboardModifiers modifiers, int delay)
{
    const QPoint centre = item->mapToScene(localPos).toPoint();
    switch (eventType) {
    case MousePress:
        QTest::mousePress(item->window(), Qt::LeftButton, modifiers, centre, delay);
        break;
    case MouseRelease:
        QTest::mouseRelease(item->window(), Qt::LeftButton, modifiers, centre, delay);
        break;
    case MouseClick:
        QTest::mouseClick(item->window(), Qt::LeftButton, modifiers, centre, delay);
        break;
    case MouseDoubleClick:
        QTest::mouseDClick(item->window(), Qt::LeftButton, modifiers, centre, delay);
        break;
    }
}

void TestHelper::wheelEvent(QQuickItem *item, const QPoint &localPos, const int degrees)
{
    QWheelEvent wheelEvent(localPos, item->window()->mapToGlobal(localPos), QPoint(0, 0), QPoint(0, degrees * 8), 0, Qt::Vertical, Qt::NoButton, 0);
    QSpontaneKeyEvent::setSpontaneous(&wheelEvent);
    if (!qApp->notify(item->window(), &wheelEvent))
        QTest::qWarn("Wheel event not accepted by receiving window");
}

bool TestHelper::changeCanvasSize(int width, int height)
{
    // Open the canvas size popup.
    mouseEventOnCentre(canvasSizeButton, MouseClick);
    const QObject *canvasSizePopup = findPopupFromTypeName("CanvasSizePopup");
    VERIFY(canvasSizePopup);
    VERIFY(canvasSizePopup->property("visible").toBool());

    // Change the values and then cancel.
    // TODO: use actual input events...
    QQuickItem *widthSpinBox = canvasSizePopup->findChild<QQuickItem*>("changeCanvasWidthSpinBox");
    VERIFY(widthSpinBox);
    // We want it to be easy to change the values with the keyboard..
    VERIFY(widthSpinBox->hasActiveFocus());
    const int originalWidthSpinBoxValue = widthSpinBox->property("value").toInt();
    VERIFY(widthSpinBox->setProperty("value", originalWidthSpinBoxValue + 1));
    VERIFY(widthSpinBox->property("value").toInt() == originalWidthSpinBoxValue + 1);

    QQuickItem *heightSpinBox = canvasSizePopup->findChild<QQuickItem*>("changeCanvasHeightSpinBox");
    const int originalHeightSpinBoxValue = heightSpinBox->property("value").toInt();
    VERIFY(heightSpinBox);
    VERIFY(heightSpinBox->setProperty("value", originalHeightSpinBoxValue - 1));
    VERIFY(heightSpinBox->property("value").toInt() == originalHeightSpinBoxValue - 1);

    QQuickItem *cancelButton = canvasSizePopup->findChild<QQuickItem*>("canvasSizePopupCancelButton");
    VERIFY(cancelButton);
    mouseEventOnCentre(cancelButton, MouseClick);
    VERIFY(!canvasSizePopup->property("visible").toBool());
    VERIFY(project->size().width() == originalWidthSpinBoxValue);
    VERIFY(project->size().height() == originalHeightSpinBoxValue);

    // Open the popup again.
    mouseEventOnCentre(canvasSizeButton, MouseClick);
    VERIFY(canvasSizePopup);
    VERIFY(canvasSizePopup->property("visible").toBool());
    // The old values should be restored.
    VERIFY(widthSpinBox->property("value").toInt() == originalWidthSpinBoxValue);
    VERIFY(heightSpinBox->property("value").toInt() == originalHeightSpinBoxValue);

    // Change the values and then press OK.
    VERIFY(widthSpinBox->setProperty("value", width));
    VERIFY(widthSpinBox->property("value").toInt() == width);
    VERIFY(heightSpinBox->setProperty("value", height));
    VERIFY(heightSpinBox->property("value").toInt() == height);

    QQuickItem *okButton = canvasSizePopup->findChild<QQuickItem*>("canvasSizePopupOkButton");
    VERIFY(okButton);
    mouseEventOnCentre(okButton, MouseClick);
    VERIFY(!canvasSizePopup->property("visible").toBool());
    VERIFY(project->size().width() == width);
    VERIFY(project->size().height() == height);
    VERIFY(widthSpinBox->property("value").toInt() == width);
    VERIFY(heightSpinBox->property("value").toInt() == height);

    return true;
}

bool TestHelper::changeImageSize(int width, int height)
{
    // Open the image size popup.
    mouseEventOnCentre(imageSizeButton, MouseClick);
    const QObject *imageSizePopup = findPopupFromTypeName("ImageSizePopup");
    VERIFY(imageSizePopup);
    VERIFY(imageSizePopup->property("visible").toBool());

    // Change the values and then cancel.
    // TODO: use actual input events...
    QQuickItem *widthSpinBox = imageSizePopup->findChild<QQuickItem*>("changeImageWidthSpinBox");
    VERIFY(widthSpinBox);
    // We want it to be easy to change the values with the keyboard..
    VERIFY(widthSpinBox->hasActiveFocus());
    const int originalWidthSpinBoxValue = widthSpinBox->property("value").toInt();
    VERIFY(widthSpinBox->setProperty("value", originalWidthSpinBoxValue + 1));
    VERIFY(widthSpinBox->property("value").toInt() == originalWidthSpinBoxValue + 1);

    QQuickItem *heightSpinBox = imageSizePopup->findChild<QQuickItem*>("changeImageHeightSpinBox");
    const int originalHeightSpinBoxValue = heightSpinBox->property("value").toInt();
    VERIFY(heightSpinBox);
    VERIFY(heightSpinBox->setProperty("value", originalHeightSpinBoxValue - 1));
    VERIFY(heightSpinBox->property("value").toInt() == originalHeightSpinBoxValue - 1);

    QQuickItem *cancelButton = imageSizePopup->findChild<QQuickItem*>("imageSizePopupCancelButton");
    VERIFY(cancelButton);
    mouseEventOnCentre(cancelButton, MouseClick);
    VERIFY(!imageSizePopup->property("visible").toBool());
    VERIFY(project->size().width() == originalWidthSpinBoxValue);
    VERIFY(project->size().height() == originalHeightSpinBoxValue);

    // Open the popup again.
    mouseEventOnCentre(imageSizeButton, MouseClick);
    VERIFY(imageSizePopup);
    VERIFY(imageSizePopup->property("visible").toBool());
    // The old values should be restored.
    VERIFY(widthSpinBox->property("value").toInt() == originalWidthSpinBoxValue);
    VERIFY(heightSpinBox->property("value").toInt() == originalHeightSpinBoxValue);

    // Change the values and then press OK.
    VERIFY(widthSpinBox->setProperty("value", width));
    VERIFY(widthSpinBox->property("value").toInt() == width);
    VERIFY(heightSpinBox->setProperty("value", height));
    VERIFY(heightSpinBox->property("value").toInt() == height);

    QQuickItem *okButton = imageSizePopup->findChild<QQuickItem*>("imageSizePopupOkButton");
    VERIFY(okButton);
    mouseEventOnCentre(okButton, MouseClick);
    VERIFY(!imageSizePopup->property("visible").toBool());
    VERIFY(project->size().width() == width);
    VERIFY(project->size().height() == height);
    VERIFY(widthSpinBox->property("value").toInt() == width);
    VERIFY(heightSpinBox->property("value").toInt() == height);

    return true;
}

bool TestHelper::changeToolSize(int size)
{
    mouseEventOnCentre(toolSizeButton, MouseClick);
    const QObject *toolSizePopup = findPopupFromTypeName("ToolSizePopup");
    VERIFY(toolSizePopup);
    VERIFY(toolSizePopup->property("visible").toBool() == true);

    QQuickItem *toolSizeSlider = toolSizePopup->findChild<QQuickItem*>("toolSizeSlider");
    VERIFY(toolSizeSlider);

    QQuickItem *toolSizeSliderHandle = toolSizeSlider->property("handle").value<QQuickItem*>();
    VERIFY(toolSizeSliderHandle);

    // Move the slider to the right to find the max pos.
    VERIFY(toolSizeSlider->setProperty("value", toolSizeSlider->property("to").toReal()));
    VERIFY(toolSizeSlider->property("value") == toolSizeSlider->property("to"));
    const QPoint handleMaxPos = toolSizeSliderHandle->mapToScene(
        QPointF(toolSizeSliderHandle->width() / 2, toolSizeSliderHandle->height() / 2)).toPoint();

    // Move/reset the slider to the left since we move from left to right.
    VERIFY(toolSizeSlider->setProperty("value", toolSizeSlider->property("from").toReal()));
    VERIFY(toolSizeSlider->property("value") == toolSizeSlider->property("from"));
    const QPoint handleMinPos = toolSizeSliderHandle->mapToScene(
        QPointF(toolSizeSliderHandle->width() / 2, toolSizeSlider->height() / 2)).toPoint();

    QPoint sliderHandlePos = handleMinPos;
    QTest::mousePress(toolSizeSlider->window(), Qt::LeftButton, Qt::NoModifier, sliderHandlePos);
    VERIFY(toolSizeSlider->property("pressed").toBool() == true);
    VERIFY(window->mouseGrabberItem() == toolSizeSlider);

    QTest::mouseMove(toolSizeSlider->window(), sliderHandlePos, 5);

    // Move the slider's handle until we find the value we want.
    for (;
         sliderValue(toolSizeSlider) != size && sliderHandlePos.x() < handleMaxPos.x();
         ++sliderHandlePos.rx()) {
        QTest::mouseMove(toolSizeSlider->window(), sliderHandlePos, 5);
    }
    --sliderHandlePos.rx();
    QTest::mouseRelease(toolSizeSlider->window(), Qt::LeftButton, Qt::NoModifier, sliderHandlePos);
    VERIFY(toolSizeSlider->property("pressed").toBool() == false);
    VERIFY(sliderValue(toolSizeSlider) == size);

    // Close the popup.
    QTest::keyClick(window, Qt::Key_Escape);
    VERIFY(toolSizePopup->property("visible").toBool() == false);

    return true;
}

bool TestHelper::moveContents(int x, int y, bool onlyVisibleLayers)
{
    const QImage originalContents = project->exportedImage();

    // Open the move contents dialog.
    VERIFY2(triggerShortcut("moveContentsShortcut", app.settings()->moveContentsShortcut()), failureMessage);
    const QObject *moveContentsDialog = findPopupFromTypeName("MoveContentsDialog");
    VERIFY(moveContentsDialog);
    VERIFY(moveContentsDialog->property("visible").toBool());

    // Change the values and then cancel.
    // TODO: use actual input events...
    QQuickItem *moveContentsXSpinBox = moveContentsDialog->findChild<QQuickItem*>("moveContentsXSpinBox");
    VERIFY(moveContentsXSpinBox);
    // We want it to be easy to change the values with the keyboard..
    VERIFY(moveContentsXSpinBox->hasActiveFocus());
    const int originalXSpinBoxValue = moveContentsXSpinBox->property("value").toInt();
    VERIFY(moveContentsXSpinBox->setProperty("value", originalXSpinBoxValue + 1));
    VERIFY(moveContentsXSpinBox->property("value").toInt() == originalXSpinBoxValue + 1);

    QQuickItem *moveContentsYSpinBox = moveContentsDialog->findChild<QQuickItem*>("moveContentsYSpinBox");
    const int originalYSpinBoxValue = moveContentsYSpinBox->property("value").toInt();
    VERIFY(moveContentsYSpinBox);
    VERIFY(moveContentsYSpinBox->setProperty("value", originalYSpinBoxValue - 1));
    VERIFY(moveContentsYSpinBox->property("value").toInt() == originalYSpinBoxValue - 1);

    QQuickItem *cancelButton = moveContentsDialog->findChild<QQuickItem*>("moveContentsDialogCancelButton");
    VERIFY(cancelButton);
    mouseEventOnCentre(cancelButton, MouseClick);
    VERIFY(!moveContentsDialog->property("visible").toBool());
    VERIFY(project->exportedImage() == originalContents);

    // Open the dialog again.
    VERIFY(triggerShortcut("moveContentsShortcut", app.settings()->moveContentsShortcut()));
    VERIFY(moveContentsDialog->property("visible").toBool());
    // The old values should be restored.
    VERIFY(moveContentsXSpinBox->property("value").toInt() == originalXSpinBoxValue);
    VERIFY(moveContentsYSpinBox->property("value").toInt() == originalYSpinBoxValue);

    // Change the values and then press OK.
    VERIFY(moveContentsXSpinBox->setProperty("value", x));
    VERIFY(moveContentsXSpinBox->property("value").toInt() == x);
    VERIFY(moveContentsYSpinBox->setProperty("value", y));
    VERIFY(moveContentsYSpinBox->property("value").toInt() == y);

    if (onlyVisibleLayers) {
        QQuickItem *onlyMoveVisibleLayersCheckBox = moveContentsDialog->findChild<QQuickItem*>("onlyMoveVisibleLayersCheckBox");
        VERIFY(onlyMoveVisibleLayersCheckBox);
        if (onlyMoveVisibleLayersCheckBox->property("checked").toBool() != onlyVisibleLayers) {
            mouseEventOnCentre(onlyMoveVisibleLayersCheckBox, MouseClick);
            VERIFY(onlyMoveVisibleLayersCheckBox->property("checked").toBool() == onlyVisibleLayers);
        }
    }

    QImage movedContents(originalContents.size(), QImage::Format_ARGB32_Premultiplied);
    movedContents.fill(Qt::transparent);

    QPainter painter(&movedContents);
    painter.drawImage(x, y, originalContents);
    painter.end();

    QQuickItem *okButton = moveContentsDialog->findChild<QQuickItem*>("moveContentsDialogOkButton");
    VERIFY(okButton);
    mouseEventOnCentre(okButton, MouseClick);
    VERIFY(!moveContentsDialog->property("visible").toBool());
    VERIFY(project->exportedImage() == movedContents);
    VERIFY(moveContentsXSpinBox->property("value").toInt() == x);
    VERIFY(moveContentsYSpinBox->property("value").toInt() == y);

    return true;
}

int TestHelper::sliderValue(QQuickItem *slider) const
{
    const qreal position = slider->property("position").toReal();
    qreal value = 0;
    QMetaObject::invokeMethod(slider, "valueAt", Qt::DirectConnection, Q_RETURN_ARG(qreal, value), Q_ARG(qreal, position));
    return qFloor(value);
}

bool TestHelper::drawPixelAtCursorPos()
{
    if (tilesetProject) {
        const Tile *targetTile = tilesetProject->tileAt(cursorPos);
        VERIFY(targetTile);

        if (!switchTool(TileCanvas::PenTool))
            return false;
        if (!switchMode(TileCanvas::PixelMode))
            return false;

        // Draw on some pixels of the current tile.
        const QImage originalTileImage = targetTile->tileset()->image()->copy(targetTile->sourceRect());
        QImage expectedImage = originalTileImage;
        expectedImage.setPixelColor(tileCanvas->scenePosToTilePixelPos(cursorPos), tileCanvas->penForegroundColour());

        QTest::mouseMove(window, cursorPos);
        QTest::mousePress(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
        VERIFY(targetTile->tileset()->image()->copy(targetTile->sourceRect()) != originalTileImage);
        VERIFY(targetTile->tileset()->image()->copy(targetTile->sourceRect()) == expectedImage);
        VERIFY(tilesetProject->hasUnsavedChanges());
        VERIFY(window->title().contains("*"));

        QTest::mouseRelease(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
        VERIFY(targetTile->tileset()->image()->copy(targetTile->sourceRect()) == expectedImage);
        VERIFY(tilesetProject->hasUnsavedChanges());
        VERIFY(window->title().contains("*"));
    } else {
        if (!switchTool(TileCanvas::PenTool))
            return false;

        QTest::mouseMove(window, cursorWindowPos);
        QTest::mousePress(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
        VERIFY(canvas->currentProjectImage()->pixelColor(cursorPos) == canvas->penForegroundColour());
        VERIFY(project->hasUnsavedChanges());
        VERIFY(window->title().contains("*"));

        QTest::mouseRelease(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
        VERIFY(canvas->currentProjectImage()->pixelColor(cursorPos) == canvas->penForegroundColour());
        VERIFY(project->hasUnsavedChanges());
        VERIFY(window->title().contains("*"));
    }

    return true;
}

bool TestHelper::drawTileAtCursorPos()
{
    if (!switchTool(TileCanvas::PenTool))
        return false;
    if (!switchMode(TileCanvas::TileMode))
        return false;

    QTest::mouseMove(window, cursorWindowPos);
    QTest::mousePress(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
    QTest::mouseRelease(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
    const int penId = tileCanvas->penTile()->id();
    VERIFY(tilesetProject->tileAt(cursorPos)->id() == penId);
    return true;
}

static QString fuzzyColourCompareFailMsg(const QColor &colour1, const QColor &colour2,
    const QChar &componentName, int difference, int fuzz)
{
    return QString::fromLatin1("colour1 %1 is not equal to colour2 %2; %3 difference of %4 is larger than fuzz of %5")
            .arg(colour1.name()).arg(colour2.name()).arg(componentName).arg(difference).arg(fuzz);
}

bool TestHelper::fuzzyColourCompare(const QColor &colour1, const QColor &colour2, int fuzz)
{
    const int rDiff = qAbs(colour2.red() - colour1.red());
    VERIFY2(rDiff <= fuzz, qPrintable(fuzzyColourCompareFailMsg(colour1, colour2, QLatin1Char('r'), rDiff, fuzz)));

    const int gDiff = qAbs(colour2.green() - colour1.green());
    VERIFY2(gDiff <= fuzz, qPrintable(fuzzyColourCompareFailMsg(colour1, colour2, QLatin1Char('g'), gDiff, fuzz)));

    const int bDiff = qAbs(colour2.blue() - colour1.blue());
    VERIFY2(bDiff <= fuzz, qPrintable(fuzzyColourCompareFailMsg(colour1, colour2, QLatin1Char('b'), bDiff, fuzz)));

    const int aDiff = qAbs(colour2.alpha() - colour1.alpha());
    VERIFY2(aDiff <= fuzz, qPrintable(fuzzyColourCompareFailMsg(colour1, colour2, QLatin1Char('a'), aDiff, fuzz)));

    return true;
}

bool TestHelper::fuzzyImageCompare(const QImage &image1, const QImage &image2)
{
    VERIFY(image1.size() == image2.size());

    for (int y = 0; y < image1.height(); ++y) {
        for (int x = 0; x < image1.width(); ++x) {
            if (!fuzzyColourCompare(image1.pixelColor(x, y), image2.pixelColor(x, y)))
                return false;
        }
    }

    return true;
}

QObject *TestHelper::findPopupFromTypeName(const QString &typeName) const
{
    QObject *popup = nullptr;
    foreach (QQuickItem *child, overlay->childItems()) {
        if (QString::fromLatin1(child->metaObject()->className()) == "QQuickPopupItem") {
            if (QString::fromLatin1(child->parent()->metaObject()->className()).contains(typeName)) {
                popup = child->parent();
                break;
            }
        }
    }
    return popup;
}

QQuickItem *TestHelper::findDialogButtonFromText(const QObject *dialog, const QString &text)
{
    QQuickItem *footer = dialog->property("footer").value<QQuickItem*>();
    if (!footer)
        return nullptr;

    return findChildWithText(footer, text);
}

QQuickItem *TestHelper::findDialogButtonFromObjectName(const QObject *dialog, const QString &objectName)
{
    QQuickItem *footer = dialog->property("footer").value<QQuickItem*>();
    if (!footer)
        return nullptr;

    return footer->findChild<QQuickItem*>(objectName);
}

QQuickItem *TestHelper::findListViewChild(QQuickItem *listView, const QString &childObjectName) const
{
    QQuickItem *listViewContentItem = listView->property("contentItem").value<QQuickItem*>();
    if (!listViewContentItem)
        return nullptr;

    QQuickItem *listViewChild = nullptr;
    foreach (QQuickItem *child, listViewContentItem->childItems()) {
        if (child->objectName() == childObjectName) {
            listViewChild = child;
            break;
        }
    }
    return listViewChild;
}

QQuickItem *TestHelper::findListViewChild(const QString &listViewObjectName, const QString &childObjectName) const
{
    QQuickItem *listView = window->findChild<QQuickItem*>(listViewObjectName);
    if (!listView)
        return nullptr;

    return findListViewChild(listView, childObjectName);
}

QQuickItem *TestHelper::findChildWithText(QQuickItem *item, const QString &text)
{
    foreach (QQuickItem *child, item->childItems()) {
        if (child->property("text").toString() == text)
            return child;
        else {
            QQuickItem *match = findChildWithText(child, text);
            if (match)
                return match;
        }
    }
    return nullptr;
}

QPoint TestHelper::mapToTile(const QPoint &cursorPos) const
{
    return cursorPos - tileCanvas->mapToScene(QPointF(0, 0)).toPoint();
}

QPoint TestHelper::tileSceneCentre(int xPosInTiles, int yPosInTiles) const
{
    return tileCanvas->mapToScene(QPointF(
        xPosInTiles * tilesetProject->tileWidth() + tilesetProject->tileWidth() / 2,
        yPosInTiles * tilesetProject->tileHeight() + tilesetProject->tileHeight() / 2)).toPoint()
            + tileCanvas->firstPane()->offset();
}

QPoint TestHelper::tileCanvasCentre(int xPosInTiles, int yPosInTiles) const
{
    return QPoint(
        xPosInTiles * tilesetProject->tileWidth() + tilesetProject->tileWidth() / 2,
                yPosInTiles * tilesetProject->tileHeight() + tilesetProject->tileHeight() / 2);
}

QPointF TestHelper::canvasCentre() const
{
    return QPointF(canvas->width() / 2, canvas->height() / 2);
}

QPoint TestHelper::canvasSceneCentre() const
{
    return canvas->mapToScene(canvasCentre()).toPoint();
}

QPoint TestHelper::firstPaneSceneCentre() const
{
    return canvas->mapToScene(QPointF((canvas->firstPane()->size() * canvas->width()) / 2, canvas->height() / 2)).toPoint();
}

void TestHelper::setCursorPosInPixels(const QPoint &posInPixels)
{
    cursorPos = posInPixels;
    cursorWindowPos = canvas->mapToScene(QPointF(posInPixels.x(), posInPixels.y())).toPoint();
}

void TestHelper::setCursorPosInTiles(int xPosInTiles, int yPosInTiles)
{
    cursorPos = tileCanvasCentre(xPosInTiles, yPosInTiles);
    cursorWindowPos = tileSceneCentre(xPosInTiles, yPosInTiles);
}

void TestHelper::setCursorPosInScenePixels(int xPosInScenePixels, int yPosInScenePixels, bool verifyWithinWindow)
{
    cursorPos = QPoint(xPosInScenePixels, yPosInScenePixels);
    cursorWindowPos = canvas->mapToScene(cursorPos).toPoint() + canvas->firstPane()->offset();
    if (verifyWithinWindow) {
        // As with mouseEventOnCentre(), we don't want this to be a e.g. VERIFY2, because then we'd have to
        // verify its return value everywhere we use it, and we use it a lot, so just assert instead.
        Q_ASSERT_X(cursorWindowPos.x() >= 0 && cursorWindowPos.y() >= 0, Q_FUNC_INFO,
            qPrintable(QString::fromLatin1("x %1 y %2").arg(cursorWindowPos.x()).arg(cursorWindowPos.y())));
    }
}

void TestHelper::setCursorPosInScenePixels(const QPoint &posInScenePixels)
{
    cursorPos = posInScenePixels;
    cursorWindowPos = canvas->mapToScene(QPointF(posInScenePixels.x(), posInScenePixels.y())).toPoint()
            + canvas->firstPane()->offset();
}

QPoint TestHelper::tilesetTileCentre(int xPosInTiles, int yPosInTiles) const
{
    return QPoint(
        xPosInTiles * tilesetProject->tileWidth() + tilesetProject->tileWidth() / 2,
        yPosInTiles * tilesetProject->tileHeight() + tilesetProject->tileHeight() / 2);
}

QPoint TestHelper::tilesetTileSceneCentre(int xPosInTiles, int yPosInTiles) const
{
    return tilesetSwatchFlickable->mapToScene(QPointF(
         xPosInTiles * tilesetProject->tileWidth() + tilesetProject->tileWidth() / 2,
         yPosInTiles * tilesetProject->tileHeight() + tilesetProject->tileHeight() / 2)).toPoint();
}

void TestHelper::keySequence(QWindow *window, QKeySequence sequence)
{
    for (int i = 0; i < sequence.count(); ++i) {
        Qt::Key key = Qt::Key(sequence[i] & ~Qt::KeyboardModifierMask);
        Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(sequence[i] & Qt::KeyboardModifierMask);
        QTest::keyClick(window, key, modifiers);
    }
}

int TestHelper::digits(int number)
{
    QVector<int> digits;
    number = qAbs(number);
    do {
        digits.push_front(number % 10);
        number /= 10;
    } while (number > 0);

    return digits.size();
}

int TestHelper::digitAt(int number, int index)
{
    QVector<int> digits;
    number = qAbs(number);
    do {
        digits.push_front(number % 10);
        number /= 10;
    } while (number > 0);

    return index < digits.size() ? digits.at(index) : 0;
}

bool TestHelper::triggerShortcut(const QString &objectName, const QString &sequenceAsString)
{
    QObject *shortcut = window->findChild<QObject*>(objectName);
    VERIFY2(shortcut, qPrintable(QString::fromLatin1("Shortcut \"%1\" could not be found").arg(objectName)));
    VERIFY2(shortcut->property("enabled").toBool(), qPrintable(QString::fromLatin1(
        "Shortcut \"%1\" is not enabled (%2 has active focus; does this shortcut require the canvas to have it?)")
            .arg(objectName, window->activeFocusItem() ? window->activeFocusItem()->objectName() : QString())));

    QSignalSpy activatedSpy(shortcut, SIGNAL(activated()));
    VERIFY(activatedSpy.isValid());

    VERIFY(QTest::qWaitForWindowActive(window));
    const int value = QKeySequence(sequenceAsString)[0];
    Qt::KeyboardModifiers mods = (Qt::KeyboardModifiers)(value & Qt::KeyboardModifierMask);
    QTest::keyClick(window, value & ~mods, mods);
    VERIFY2(activatedSpy.count() == 1, qPrintable(QString::fromLatin1(
        "The activated() signal was not emitted for %1 with sequence %2").arg(objectName).arg(sequenceAsString)));

    return true;
}

bool TestHelper::triggerNewProject()
{
    return triggerShortcut("newShortcut", app.settings()->newShortcut());
}

bool TestHelper::triggerCloseProject()
{
    return triggerShortcut("closeShortcut", app.settings()->closeShortcut());
}

bool TestHelper::triggerSaveProject()
{
    return triggerShortcut("saveShortcut", app.settings()->saveShortcut());
}

bool TestHelper::triggerSaveProjectAs()
{
    FAIL("TODO: no saveas shortcut");
//    triggerShortcut(app.settings()->saveShortcut());
}

bool TestHelper::triggerOpenProject()
{
    return triggerShortcut("openShortcut", app.settings()->openShortcut());
}

bool TestHelper::triggerRevert()
{
    return triggerShortcut("revertShortcut", app.settings()->revertShortcut());
}

bool TestHelper::triggerPaste()
{
    return triggerShortcut("pasteShortcut", QKeySequence(QKeySequence::Paste).toString());
}

bool TestHelper::triggerFlipHorizontally()
{
    return triggerShortcut("flipHorizontallyShortcut", app.settings()->flipHorizontallyShortcut());
}

bool TestHelper::triggerFlipVertically()
{
    return triggerShortcut("flipVerticallyShortcut", app.settings()->flipVerticallyShortcut());
}

bool TestHelper::triggerCentre()
{
    return triggerShortcut("centreShortcut", app.settings()->centreShortcut());
}

bool TestHelper::triggerGridVisible()
{
    return triggerShortcut("gridVisibleShortcut", app.settings()->gridVisibleShortcut());
}

bool TestHelper::triggerRulersVisible()
{
    return triggerShortcut("rulersVisibleShortcut", app.settings()->rulersVisibleShortcut());
}

bool TestHelper::triggerGuidesVisible()
{
    return triggerShortcut("guidesVisibleShortcut", app.settings()->guidesVisibleShortcut());
}

bool TestHelper::triggerSplitScreen()
{
    return triggerShortcut("splitScreenShortcut", app.settings()->splitScreenShortcut());
}

bool TestHelper::triggerSplitterLocked()
{
    return triggerShortcut("splitterLockedShortcut", app.settings()->splitterLockedShortcut());
}

bool TestHelper::setSplitterLocked(bool splitterLocked)
{
    if (canvas->splitter()->isEnabled() != !splitterLocked) {
        if (!triggerSplitterLocked())
            return false;
        VERIFY(canvas->splitter()->isEnabled() == !splitterLocked);
    }
    return true;
}

bool TestHelper::triggerAnimationPlayback()
{
    return triggerShortcut("animationPlaybackShortcut", app.settings()->animationPlaybackShortcut());
}

bool TestHelper::setAnimationPlayback(bool usingAnimation)
{
    if (layeredImageProject->isUsingAnimation() != usingAnimation) {
        if (!triggerAnimationPlayback())
            return false;
        VERIFY(layeredImageProject->isUsingAnimation() == usingAnimation);
    }
    return true;
}

bool TestHelper::triggerOptions()
{
    return triggerShortcut("optionsShortcut", app.settings()->optionsShortcut());
}

bool TestHelper::selectLayer(const QString &layerName, int layerIndex)
{
    TRY_VERIFY(findListViewChild("layerListView", layerName));
    QQuickItem *layerDelegate = findListViewChild("layerListView", layerName);
    VERIFY(layerDelegate);
    mouseEventOnCentre(layerDelegate, MouseClick);
    VERIFY(layerDelegate->property("checked").toBool());
    VERIFY(layeredImageProject->currentLayerIndex() == layerIndex);
    return true;
}

bool TestHelper::verifyLayerName(const QString &layerName, QQuickItem **layerDelegate)
{
    // It seems that the ListView sometimes need some extra time to create the second item (e.g. when debugging).
    TRY_VERIFY(findListViewChild("layerListView", layerName));
    QQuickItem *delegate = findListViewChild("layerListView", layerName);
    VERIFY(delegate);
    QQuickItem *layerDelegateNameTextField = delegate->findChild<QQuickItem*>("layerNameTextField");
    VERIFY(layerDelegateNameTextField);
    VERIFY(layerDelegateNameTextField->property("text").toString() == layerName);
    if (layerDelegate)
        *layerDelegate = delegate;
    return true;
}

void TestHelper::addAllProjectTypes()
{
    QTest::addColumn<Project::Type>("projectType");

    QTest::newRow("TilesetType") << Project::TilesetType;
    QTest::newRow("ImageType") << Project::ImageType;
    QTest::newRow("LayeredImageType") << Project::LayeredImageType;
}

void TestHelper::addImageProjectTypes()
{
    QTest::addColumn<Project::Type>("projectType");

    QTest::newRow("ImageType") << Project::ImageType;
    QTest::newRow("LayeredImageType") << Project::LayeredImageType;
}

void TestHelper::addActualProjectTypes()
{
    QTest::addColumn<Project::Type>("projectType");
    QTest::addColumn<QString>("projectExtension");

    QTest::newRow("TilesetType") << Project::TilesetType << "stp";
    QTest::newRow("LayeredImageType") << Project::LayeredImageType << "slp";
}

bool TestHelper::createNewProject(Project::Type projectType, const QVariantMap &args)
{
    const bool isTilesetProject = projectType == Project::TilesetType;

    // tileset args
    const int tileWidth = isTilesetProject ? args.value("tileWidth", 25).toInt() : 0;
    const int tileHeight = isTilesetProject ? args.value("tileHeight", 25).toInt() : 0;
    const int tilesetTilesWide = isTilesetProject ? args.value("tilesetTilesWide", -1).toInt() : 0;
    const int tilesetTilesHigh = isTilesetProject ? args.value("tilesetTilesHigh", -1).toInt() : 0;
    const bool transparentTilesetBackground = isTilesetProject ? args.value("transparentTilesetBackground", false).toBool() : false;

    // image args
    const int imageWidth = !isTilesetProject ? args.value("imageWidth", 256).toInt() : 0;
    const int imageHeight = !isTilesetProject ? args.value("imageHeight", 256).toInt() : 0;
    const bool transparentImageBackground = !isTilesetProject ? args.value("transparentImageBackground", false).toBool() : false;

    if (!project) {
        // Hovering over the canvas with no project open should result in the default cursor being displayed.
        QTest::mouseMove(window, QPointF(window->width() / 2, window->height() / 2).toPoint());
        VERIFY(window->cursor().shape() == Qt::ArrowCursor);
    }

    if (creationErrorOccurredSpy)
        creationErrorOccurredSpy->clear();

    // Click the new project button.
    if (!triggerNewProject())
        return false;

    // Check that we get prompted to discard any changes.
    if (project && project->hasUnsavedChanges()) {
        const QObject *discardChangesDialog = window->contentItem()->findChild<QObject*>("discardChangesDialog");
        VERIFY(discardChangesDialog);
        VERIFY(discardChangesDialog->property("visible").toBool());

        QQuickItem *discardChangesButton = findDialogButtonFromObjectName(discardChangesDialog, "discardChangesDialogButton");
        VERIFY(discardChangesButton);
        mouseEventOnCentre(discardChangesButton, MouseClick);
        VERIFY(!discardChangesDialog->property("visible").toBool());
    }

    // Ensure that the new project popup is visible.
    const QObject *newProjectPopup = findPopupFromTypeName("NewProjectPopup");
    VERIFY(newProjectPopup);
    VERIFY(newProjectPopup->property("visible").toBool());
    // TODO: remove this when https://bugreports.qt.io/browse/QTBUG-53420 is fixed
    newProjectPopup->property("contentItem").value<QQuickItem*>()->forceActiveFocus();
    VERIFY2(newProjectPopup->property("activeFocus").toBool(),
        qPrintable(QString::fromLatin1("NewProjectPopup doesn't have active focus (%1 does)").arg(window->activeFocusItem()->objectName())));

    QString newProjectButtonObjectName;
    if (projectType == Project::TilesetType) {
        newProjectButtonObjectName = QLatin1String("tilesetProjectButton");
    } else if (projectType == Project::ImageType) {
        newProjectButtonObjectName = QLatin1String("imageProjectButton");
    } else {
        newProjectButtonObjectName = QLatin1String("layeredImageProjectButton");
    }

    // Click on the appropriate project type button.
    QQuickItem *tilesetProjectButton = newProjectPopup->findChild<QQuickItem*>(newProjectButtonObjectName);
    VERIFY(tilesetProjectButton);

    mouseEventOnCentre(tilesetProjectButton, MouseClick);
    VERIFY(tilesetProjectButton->property("checked").toBool());

    TRY_VERIFY(!newProjectPopup->property("visible").toBool());

    if (projectType == Project::TilesetType) {
        // Create a temporary directory containing a tileset image for us to use.
        if (!setupTempTilesetProjectDir())
            return false;

        // Now the New Tileset Project popup should be visible.
        TRY_VERIFY(findPopupFromTypeName("NewTilesetProjectPopup"));
        const QObject *newTilesetProjectPopup = findPopupFromTypeName("NewTilesetProjectPopup");
        VERIFY(newTilesetProjectPopup->property("visible").toBool());

        // Ensure that the popup gets reset each time it's opened.
        QQuickItem *useExistingTilesetCheckBox = newTilesetProjectPopup->findChild<QQuickItem*>("useExistingTilesetCheckBox");
        VERIFY(useExistingTilesetCheckBox);
        VERIFY(useExistingTilesetCheckBox->property("checked").isValid());
        VERIFY(!useExistingTilesetCheckBox->property("checked").toBool());

        QQuickItem *tilesetField = newTilesetProjectPopup->findChild<QQuickItem*>("tilesetPathTextField");
        VERIFY(tilesetField);
        VERIFY(tilesetField->property("text").isValid());
        VERIFY(tilesetField->property("text").toString().isEmpty());

        QQuickItem *tileWidthSpinBox = newTilesetProjectPopup->findChild<QQuickItem*>("tileWidthSpinBox");
        VERIFY(tileWidthSpinBox);
        VERIFY(tileWidthSpinBox->property("value").toInt() == 32);

        QQuickItem *tileHeightSpinBox = newTilesetProjectPopup->findChild<QQuickItem*>("tileHeightSpinBox");
        VERIFY(tileHeightSpinBox);
        VERIFY(tileHeightSpinBox->property("value").toInt() == 32);

        QQuickItem *tilesWideSpinBox = newTilesetProjectPopup->findChild<QQuickItem*>("tilesWideSpinBox");
        VERIFY(tilesWideSpinBox);
        VERIFY(tilesWideSpinBox->property("value").toInt() == 10);

        QQuickItem *tilesHighSpinBox = newTilesetProjectPopup->findChild<QQuickItem*>("tilesHighSpinBox");
        VERIFY(tilesHighSpinBox);
        VERIFY(tilesHighSpinBox->property("value").toInt() == 10);

        VERIFY(tileWidthSpinBox->setProperty("value", tileWidth));
        VERIFY(tileWidthSpinBox->property("value").toInt() == tileWidth);

        VERIFY(tileHeightSpinBox->setProperty("value", tileHeight));
        VERIFY(tileHeightSpinBox->property("value").toInt() == tileHeight);

        if (tilesetTilesWide == -1 || tilesetTilesHigh == -1) {
            // Using an existing tileset image.

            // Check the "use existing tileset" checkbox.
            // Hack: it has focus in the app, but not in the test => meh
            useExistingTilesetCheckBox->forceActiveFocus();
            VERIFY(useExistingTilesetCheckBox->hasActiveFocus());
            QTest::keyClick(window, Qt::Key_Space);
            VERIFY(useExistingTilesetCheckBox->property("checked").toBool());

            // Input the filename.
            QTest::keyClick(window, Qt::Key_Tab);
            VERIFY(tilesetField->hasActiveFocus());
            // TODO: input path using events
            VERIFY(tilesetField->property("text").isValid());
            VERIFY(tilesetField->setProperty("text", tempTilesetUrl.toString()));

            QQuickItem *invalidFileIcon = newTilesetProjectPopup->findChild<QQuickItem*>("invalidFileIcon");
            VERIFY(invalidFileIcon);
            QObject *fileValidator = newTilesetProjectPopup->findChild<QObject*>("validator");
            VERIFY2(qFuzzyCompare(invalidFileIcon->property("opacity").toReal(), 0),
                qPrintable(QString::fromLatin1("File should be valid, but got error message \"%1\"; file path: %2")
                    .arg(fileValidator->property("errorMessage").toString())
                    .arg(tilesetField->property("text").toString())));
        } else {
            // Using a new tileset image.
            VERIFY(tilesetTilesWide > 0 && tilesetTilesHigh > 0);

            VERIFY(tilesWideSpinBox->setProperty("value", tilesetTilesWide));
            VERIFY(tilesWideSpinBox->property("value").toInt() == tilesetTilesWide);

            VERIFY(tilesHighSpinBox->setProperty("value", tilesetTilesHigh));
            VERIFY(tilesHighSpinBox->property("value").toInt() == tilesetTilesHigh);

            QQuickItem *transparentBackgroundCheckBox = newTilesetProjectPopup->findChild<QQuickItem*>("transparentBackgroundCheckBox");
            VERIFY(transparentBackgroundCheckBox);
            VERIFY(transparentBackgroundCheckBox->property("checked").toBool());

            if (!transparentTilesetBackground) {
                mouseEventOnCentre(transparentBackgroundCheckBox, MouseClick);
                VERIFY(!transparentBackgroundCheckBox->property("checked").toBool());
            }
        }

        // Confirm creation of the project.
        QQuickItem *okButton = newTilesetProjectPopup->findChild<QQuickItem*>("newTilesetProjectOkButton");
        VERIFY(okButton);
        mouseEventOnCentre(okButton, MouseClick);
        VERIFY(!newTilesetProjectPopup->property("visible").toBool());
    } else {
        // Create a temporary directory that we can save into, etc.
        if (projectType == Project::LayeredImageType) {
            if (!setupTempLayeredImageProjectDir())
                return false;
        }

        // Now the New Image Project popup should be visible.
        TRY_VERIFY(findPopupFromTypeName("NewImageProjectPopup"));
        const QObject *newImageProjectPopup = findPopupFromTypeName("NewImageProjectPopup");
        VERIFY(newImageProjectPopup->property("visible").toBool());

        // Ensure that the popup gets reset each time it's opened.
        QQuickItem *imageWidthSpinBox = newImageProjectPopup->findChild<QQuickItem*>("imageWidthSpinBox");
        VERIFY(imageWidthSpinBox);
        VERIFY(imageWidthSpinBox->property("value").toInt() == 256);

        QQuickItem *imageHeightSpinBox = newImageProjectPopup->findChild<QQuickItem*>("imageHeightSpinBox");
        VERIFY(imageHeightSpinBox);
        VERIFY(imageHeightSpinBox->property("value").toInt() == 256);

        VERIFY(imageWidthSpinBox->setProperty("value", imageWidth));
        VERIFY(imageWidthSpinBox->property("value").toInt() == imageWidth);

        VERIFY(imageHeightSpinBox->setProperty("value", imageHeight));
        VERIFY(imageHeightSpinBox->property("value").toInt() == imageHeight);

        QQuickItem *transparentImageBackgroundCheckBox = newImageProjectPopup->findChild<QQuickItem*>("transparentImageBackgroundCheckBox");
        VERIFY(transparentImageBackgroundCheckBox);
        VERIFY(transparentImageBackgroundCheckBox->property("checked").isValid());
        VERIFY(!transparentImageBackgroundCheckBox->property("checked").toBool());

        if (transparentImageBackground) {
            mouseEventOnCentre(transparentImageBackgroundCheckBox, MouseClick);
            VERIFY(transparentImageBackgroundCheckBox->property("checked").toBool() == transparentImageBackground);
        }

        // Confirm creation of the project.
        QQuickItem *okButton = newImageProjectPopup->findChild<QQuickItem*>("newImageProjectOkButton");
        VERIFY(okButton);
        mouseEventOnCentre(okButton, MouseClick);
        VERIFY(!newImageProjectPopup->property("visible").toBool());
    }

    return updateVariables(true, projectType);
}

bool TestHelper::createNewTilesetProject(int tileWidth, int tileHeight, int tilesetTilesWide, int tilesetTilesHigh,
    bool transparentTilesetBackground)
{
    QVariantMap args;
    args.insert("tileWidth", tileWidth);
    args.insert("tileHeight", tileHeight);
    args.insert("tilesetTilesWide", tilesetTilesWide);
    args.insert("tilesetTilesHigh", tilesetTilesHigh);
    args.insert("transparentTilesetBackground", transparentTilesetBackground);
    return createNewProject(Project::TilesetType, args);
}

bool TestHelper::createNewImageProject(int imageWidth, int imageHeight, bool transparentImageBackground)
{
    QVariantMap args;
    args.insert("imageWidth", imageWidth);
    args.insert("imageHeight", imageHeight);
    args.insert("transparentImageBackground", transparentImageBackground);
    return createNewProject(Project::ImageType, args);
}

bool TestHelper::createNewLayeredImageProject(int imageWidth, int imageHeight, bool transparentImageBackground)
{
    QVariantMap args;
    args.insert("imageWidth", imageWidth);
    args.insert("imageHeight", imageHeight);
    args.insert("transparentImageBackground", transparentImageBackground);
    if (!createNewProject(Project::LayeredImageType, args))
        return false;

    VERIFY(layeredImageProject->layerCount() == 1);
    VERIFY(layeredImageProject->currentLayerIndex() == 0);
    VERIFY(layeredImageProject->currentLayer());
    VERIFY(layeredImageProject->layerAt(0)->name() == QLatin1String("Layer 1"));

    {
        // Ensure that what the user sees (the delegate) is correct.
        QQuickItem *layer1Delegate = findListViewChild("layerListView", QLatin1String("Layer 1"));
        VERIFY(layer1Delegate);

        QQuickItem *nameTextField = layer1Delegate->findChild<QQuickItem*>("layerNameTextField");
        VERIFY(nameTextField);
        VERIFY(nameTextField->property("text").toString() == QLatin1String("Layer 1"));
    }

    newLayerButton = window->findChild<QQuickItem*>("newLayerButton");
    VERIFY(newLayerButton);

    moveLayerUpButton = window->findChild<QQuickItem*>("moveLayerUpButton");
    VERIFY(moveLayerUpButton);

    moveLayerDownButton = window->findChild<QQuickItem*>("moveLayerDownButton");
    VERIFY(moveLayerDownButton);

    animationPlayPauseButton = window->findChild<QQuickItem*>("animationPlayPauseButton");
    VERIFY(animationPlayPauseButton);

    return true;
}

bool TestHelper::loadProject(const QUrl &url)
{
    if (creationErrorOccurredSpy)
        creationErrorOccurredSpy->clear();

    QWindow *window = qobject_cast<QWindow*>(app.qmlEngine()->rootObjects().first());
    VERIFY(window);

    VERIFY(QMetaObject::invokeMethod(window, "loadProject", Qt::DirectConnection, Q_ARG(QVariant, url)));
    VERIFY_NO_CREATION_ERRORS_OCCURRED();

    return updateVariables(false, projectManager->projectTypeForUrl(url));
}

bool TestHelper::updateVariables(bool isNewProject, Project::Type newProjectType)
{
    // The projects and canvases that we had references to should have
    // been destroyed by now.
    TRY_VERIFY(!project);
    VERIFY(!imageProject);
    VERIFY(!tilesetProject);

    TRY_VERIFY(!canvas);
    VERIFY(!imageCanvas);
    VERIFY(!tileCanvas);

    project = projectManager->project();
    VERIFY(project);

    canvas = window->findChild<ImageCanvas*>();
    VERIFY(canvas);
    // The old default was to split the screen,
    // and so the tests might be depending on it to be split.
    // Also, it's good to ensure that it's tested.
    canvas->setSplitScreen(true);
    VERIFY(canvas->isSplitScreen());

    if (newProjectType == Project::TilesetType) {
        tilesetProject = qobject_cast<TilesetProject*>(project);
        VERIFY(tilesetProject);

        tileCanvas = qobject_cast<TileCanvas*>(canvas);
        VERIFY(tileCanvas);
    } else if (newProjectType == Project::ImageType) {
        imageProject = qobject_cast<ImageProject*>(project);
        VERIFY(imageProject);

        imageCanvas = canvas;
    } else if (newProjectType == Project::LayeredImageType) {
        layeredImageProject = qobject_cast<LayeredImageProject*>(project);
        VERIFY(layeredImageProject);

        layeredImageCanvas = qobject_cast<LayeredImageCanvas*>(canvas);;
    }

    canvas->forceActiveFocus();
//    TRY_VERIFY(canvas->hasActiveFocus());

    VERIFY(project->hasLoaded());

    if (isNewProject) {
        VERIFY(project->url() == QUrl());
        VERIFY(project->isNewProject());

        // Reset any settings that have changed back to their defaults.
        QVariant settingsAsVariant = qmlEngine(canvas)->rootContext()->contextProperty("settings");
        VERIFY(settingsAsVariant.isValid());
        ApplicationSettings *settings = settingsAsVariant.value<ApplicationSettings*>();
        VERIFY(settings);
        settings->resetShortcutsToDefaults();

        if (settings->areRulersVisible()) {
            if (!triggerRulersVisible())
                return false;
            VERIFY(settings->areRulersVisible() == false);
        }

        cursorPos = QPoint();
        cursorWindowPos = QPoint();
    }

    // Sanity check.
    TRY_VERIFY(canvas->window() == canvasSizeButton->window());
    VERIFY(!canvas->splitter()->isPressed());
    VERIFY(!canvas->splitter()->isHovered());

    if (isNewProject) {
        VERIFY(canvas->splitter()->position() == 0.5);
    }

    if (newProjectType == Project::TilesetType) {
        // Establish references to TilesetProject-specific properties.
        tilesetSwatch = window->findChild<QQuickItem*>("tilesetSwatch");
        VERIFY(tilesetSwatch);
        VERIFY(tilesetSwatch->isVisible() == true);
        VERIFY(!qFuzzyIsNull(tilesetSwatch->width()));
        VERIFY(!qFuzzyIsNull(tilesetSwatch->height()));

        // Ensure that the tileset swatch flickable has the correct contentY.
        tilesetSwatchFlickable = tilesetSwatch->findChild<QQuickItem*>("tilesetSwatchFlickable");
        VERIFY(tilesetSwatchFlickable);
        VERIFY(tilesetSwatchFlickable->property("contentY").isValid());
        VERIFY(tilesetSwatchFlickable->property("contentY").toReal() == 0.0);

        VERIFY(imageGrabber.requestImage(tileCanvas));
        TRY_VERIFY(imageGrabber.isReady());
        VERIFY(imageGrabber.takeImage().pixelColor(16, 16) == tileCanvas->mapBackgroundColour());
    } else {
        VERIFY(window->findChild<QQuickItem*>("selectionToolButton"));
    }

    return true;
}

bool TestHelper::setupTempTilesetProjectDir()
{
    QStringList toCopy;
    toCopy << tilesetBasename;
    // More stuff here.

    QStringList copiedPaths;
    if (!setupTempProjectDir(toCopy, &copiedPaths))
        return false;

    tempTilesetUrl = QUrl::fromLocalFile(copiedPaths.at(0));
    // More stuff here.

    return true;
}

bool TestHelper::setupTempLayeredImageProjectDir()
{
    return setupTempProjectDir();
}

bool TestHelper::setupTempProjectDir(const QStringList &resourceFilesToCopy, QStringList *filesCopied)
{
    tempProjectDir.reset(new QTemporaryDir);
    VERIFY2(tempProjectDir->isValid(), qPrintable(tempProjectDir->errorString()));

    foreach (const QString &basename, resourceFilesToCopy) {
        QFile sourceFile(":/resources/" + basename);
        VERIFY2(sourceFile.open(QIODevice::ReadOnly), qPrintable(QString::fromLatin1(
            "Failed to open %1: %2").arg(sourceFile.fileName()).arg(sourceFile.errorString())));
        sourceFile.close();

        const QString saveFilePath = tempProjectDir->path() + "/" + basename;
        VERIFY2(QFile::copy(sourceFile.fileName(), saveFilePath), qPrintable(QString::fromLatin1(
            "Failed to copy %1 to %2: %3").arg(sourceFile.fileName()).arg(saveFilePath).arg(sourceFile.errorString())));

        // A file copied from a file that is part of resources will always be read-only...
        QFile copiedFile(saveFilePath);
        VERIFY(copiedFile.size() > 0);

        VERIFY(copiedFile.setPermissions(QFile::ReadUser | QFile::WriteUser));
        VERIFY(copiedFile.size() > 0);
        VERIFY2(copiedFile.open(QIODevice::ReadWrite), qPrintable(QString::fromLatin1(
            "Error opening file at %1: %2").arg(saveFilePath).arg(copiedFile.errorString())));

        if (filesCopied)
            *filesCopied << saveFilePath;
    }

    return true;
}

bool TestHelper::switchMode(TileCanvas::Mode mode)
{
    if (tileCanvas->mode() == mode)
        return true;

    mouseEventOnCentre(modeToolButton, MouseClick);
    const bool expectChecked = mode == TileCanvas::TileMode;
    VERIFY2(modeToolButton->property("checked").toBool() == expectChecked, qPrintable(QString::fromLatin1(
        "modeToolButton.checked should be %1, but it's %2 (trying to set mode %3)")
            .arg(expectChecked).arg(modeToolButton->property("checked").toBool()).arg(mode)));
    VERIFY(tileCanvas->mode() == mode);
    return true;
}

bool TestHelper::switchTool(ImageCanvas::Tool tool)
{
    if (canvas->tool() == tool)
        return true;

    if (tool == ImageCanvas::PenTool) {
        mouseEventOnCentre(penToolButton, MouseClick);
    } else if (tool == ImageCanvas::EyeDropperTool) {
        mouseEventOnCentre(eyeDropperToolButton, MouseClick);
    } else if (tool == ImageCanvas::FillTool) {
        mouseEventOnCentre(fillToolButton, MouseClick);
    } else if (tool == ImageCanvas::EraserTool) {
        mouseEventOnCentre(eraserToolButton, MouseClick);
    } else if (tool == ImageCanvas::SelectionTool) {
        mouseEventOnCentre(selectionToolButton, MouseClick);
    }
    VERIFY(canvas->tool() == tool);
    return true;
}

bool TestHelper::setPenForegroundColour(QString argbString)
{
    QQuickItem *hexTextField = window->findChild<QQuickItem*>("hexTextField");
    VERIFY(hexTextField);

    argbString.replace("#", QString());

    hexTextField->forceActiveFocus();
    QMetaObject::invokeMethod(hexTextField, "selectAll", Qt::DirectConnection);

    for (int i = 0; i < argbString.length(); ++i) {
        QTest::keyClick(window, argbString.at(i).toLatin1());
    }
    QTest::keyClick(window, Qt::Key_Return);
    VERIFY(hexTextField->property("text").toString() == argbString);
    return true;
}

bool TestHelper::panTopLeftTo(int x, int y)
{
    const QPoint panDistance = QPoint(x, y) - canvas->firstPane()->offset();
    return panBy(panDistance.x(), panDistance.y());
}

bool TestHelper::panBy(int xDistance, int yDistance)
{
    QPoint pressPos = firstPaneSceneCentre();
    QTest::mouseMove(window, pressPos);
    VERIFY(canvas->currentPane() == canvas->firstPane());

    // TODO: get image checks working
    //        VERIFY(imageGrabber.requestImage(canvas));
    //        QTRY_VERIFY(imageGrabber.isReady());
    //        const QImage originalImage = imageGrabber.takeImage();

    const QPoint originalOffset = canvas->currentPane()->offset();
    const QPoint expectedOffset = originalOffset + QPoint(xDistance, yDistance);

    QTest::keyPress(window, Qt::Key_Space);
    VERIFY(window->cursor().shape() == Qt::OpenHandCursor);
    VERIFY(canvas->currentPane()->offset() == originalOffset);
    //        VERIFY(imageGrabber.requestImage(canvas));
    //        QTRY_VERIFY(imageGrabber.isReady());
    //        // Cursor changed to OpenHandCursor.
    //        QImage currentImage = imageGrabber.takeImage();
    //        VERIFY(currentImage != originalImage);
    //        QImage lastImage = currentImage;

    QTest::mousePress(window, Qt::LeftButton, Qt::NoModifier, pressPos);
    VERIFY(window->cursor().shape() == Qt::ClosedHandCursor);
    VERIFY(canvas->currentPane()->offset() == originalOffset);
    //        VERIFY(imageGrabber.requestImage(canvas));
    //        QTRY_VERIFY(imageGrabber.isReady());
    //        currentImage = imageGrabber.takeImage();
    //        // Cursor changed to ClosedHandCursor.
    //        VERIFY(currentImage != lastImage);
    //        lastImage = currentImage;

    QTest::mouseMove(window, pressPos + QPoint(xDistance, yDistance));
    VERIFY(window->cursor().shape() == Qt::ClosedHandCursor);
    VERIFY(canvas->currentPane()->offset() == expectedOffset);
    //        VERIFY(imageGrabber.requestImage(canvas));
    //        // Pane offset changed.
    //        currentImage = imageGrabber.takeImage();
    //        VERIFY(currentImage != lastImage);
    //        lastImage = currentImage;

    QTest::mouseRelease(window, Qt::LeftButton, Qt::NoModifier, pressPos + QPoint(xDistance, yDistance));
    VERIFY(window->cursor().shape() == Qt::OpenHandCursor);
    VERIFY(canvas->currentPane()->offset() == expectedOffset);

    QTest::keyRelease(window, Qt::Key_Space);
    VERIFY(window->cursor().shape() == Qt::BlankCursor);
    VERIFY(canvas->currentPane()->offset() == expectedOffset);

    return true;
}

bool TestHelper::zoomTo(int zoomLevel)
{
    return zoomTo(zoomLevel, cursorWindowPos);
}

bool TestHelper::zoomTo(int zoomLevel, const QPoint &pos)
{
    CanvasPane *currentPane = canvas->currentPane();
    for (int i = 0; currentPane->zoomLevel() < zoomLevel; ++i)
        wheelEvent(canvas, pos, 1);
    VERIFY(currentPane->integerZoomLevel() == zoomLevel);
    return true;
}
