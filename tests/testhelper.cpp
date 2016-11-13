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

TestHelper::TestHelper(int &argc, char **argv) :
    app(argc, argv, QStringLiteral("Tile Editor Test Suite")),
    window(qobject_cast<QQuickWindow*>(app.qmlEngine()->rootObjects().first())),
    overlay(nullptr),
    project(window->contentItem()->findChild<Project*>()),
    canvas(window->findChild<TileCanvas*>()),
    fileToolButton(nullptr),
    optionsToolButton(nullptr),
    viewToolButton(nullptr),
    canvasSizeButton(nullptr),
    modeToolButton(nullptr),
    penToolButton(nullptr),
    eyeDropperToolButton(nullptr),
    fillToolButton(nullptr),
    toolSizeButton(nullptr),
    newMenuButton(nullptr),
    closeMenuButton(nullptr),
    openMenuButton(nullptr),
    saveMenuButton(nullptr),
    saveAsMenuButton(nullptr),
    settingsMenuButton(nullptr),
    undoButton(nullptr),
    redoButton(nullptr),
    revertMenuButton(nullptr),
    centreMenuButton(nullptr),
    showGridMenuButton(nullptr),
    splitScreenMenuButton(nullptr),
    penForegroundColourButton(nullptr),
    penBackgroundColourButton(nullptr),
    tilesetSwatch(nullptr),
    duplicateTileMenuButton(nullptr),
    rotateTileLeftMenuButton(nullptr),
    rotateTileRightMenuButton(nullptr),
    errorOccurredSpy(project, SIGNAL(errorOccurred(QString))),
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
    app.settings()->setSplitScreen(app.settings()->defaultSplitScreen());
    QVERIFY(app.settings()->isSplitScreen());

    // However, this should never change.
    QVERIFY(!app.settings()->loadLastOnStartup());

    QVERIFY(window->property("overlay").isValid());
    overlay = window->property("overlay").value<QQuickItem*>();
    QVERIFY(overlay);

    QVERIFY(project);
    QVERIFY(canvas);

    fileToolButton = window->findChild<QQuickItem*>("fileToolButton");
    QVERIFY(fileToolButton);

    optionsToolButton = window->findChild<QQuickItem*>("optionsToolButton");
    QVERIFY(optionsToolButton);

    viewToolButton = window->findChild<QQuickItem*>("viewToolButton");
    QVERIFY(viewToolButton);

    canvasSizeButton = window->findChild<QQuickItem*>("canvasSizeButton");
    QVERIFY(canvasSizeButton);

    modeToolButton = window->findChild<QQuickItem*>("modeToolButton");
    QVERIFY(modeToolButton);

    penToolButton = window->findChild<QQuickItem*>("penToolButton");
    QVERIFY(penToolButton);

    eyeDropperToolButton = window->findChild<QQuickItem*>("eyeDropperToolButton");
    QVERIFY(eyeDropperToolButton);

    fillToolButton = window->findChild<QQuickItem*>("fillToolButton");
    QVERIFY(fillToolButton);

    toolSizeButton = window->findChild<QQuickItem*>("toolSizeButton");
    QVERIFY(toolSizeButton);

    newMenuButton = window->findChild<QQuickItem*>("newMenuButton");
    QVERIFY(newMenuButton);

    closeMenuButton = window->findChild<QQuickItem*>("closeMenuButton");
    QVERIFY(closeMenuButton);

    openMenuButton = window->findChild<QQuickItem*>("openMenuButton");
    QVERIFY(openMenuButton);

    saveMenuButton = window->findChild<QQuickItem*>("saveMenuButton");
    QVERIFY(saveMenuButton);

    saveAsMenuButton = window->findChild<QQuickItem*>("saveAsMenuButton");
    QVERIFY(saveAsMenuButton);

    settingsMenuButton = window->findChild<QQuickItem*>("settingsMenuButton");
    QVERIFY(settingsMenuButton);

    undoButton = window->findChild<QQuickItem*>("undoButton");
    QVERIFY(undoButton);

    redoButton = window->findChild<QQuickItem*>("redoButton");
    QVERIFY(redoButton);

    revertMenuButton = window->findChild<QQuickItem*>("revertMenuButton");
    QVERIFY(revertMenuButton);

    centreMenuButton = window->findChild<QQuickItem*>("centreMenuButton");
    QVERIFY(centreMenuButton);

    showGridMenuButton = window->findChild<QQuickItem*>("showGridMenuButton");
    QVERIFY(showGridMenuButton);

    splitScreenMenuButton = window->findChild<QQuickItem*>("splitScreenMenuButton");
    QVERIFY(splitScreenMenuButton);

    penForegroundColourButton = window->findChild<QQuickItem*>("penForegroundColourButton");
    QVERIFY(penForegroundColourButton);

    penBackgroundColourButton = window->findChild<QQuickItem*>("penBackgroundColourButton");
    QVERIFY(penBackgroundColourButton);

    tilesetSwatch = window->findChild<QQuickItem*>("tilesetSwatch");
    QVERIFY(tilesetSwatch);

    duplicateTileMenuButton = window->findChild<QQuickItem*>("duplicateTileMenuButton");
    QVERIFY(duplicateTileMenuButton);

    rotateTileLeftMenuButton = window->findChild<QQuickItem*>("rotateTileLeftMenuButton");
    QVERIFY(rotateTileLeftMenuButton);

    rotateTileRightMenuButton = window->findChild<QQuickItem*>("rotateTileRightMenuButton");
    QVERIFY(rotateTileRightMenuButton);
}

void TestHelper::init()
{
    VERIFY_NO_ERRORS_OCCURRED();
    const QObject *errorPopup = findPopupFromTypeName("ErrorPopup");
    if (errorPopup) {
        QVERIFY2(!errorPopup->property("visible").toBool(),
            qPrintable(QString::fromLatin1("Shouldn't be any errors on startup. Error is: %1")
                 .arg(errorPopup->property("text").toString())));
    }

    // There is no "defaults" test, because it's hard/hacky to predict what the lastProjectUrl will be.
    if (project->hasLoaded()) {
        project->close();
        VERIFY_NO_ERRORS_OCCURRED();
        QVERIFY(!project->hasLoaded());
    }

    canvas->forceActiveFocus();
    QTRY_VERIFY(canvas->hasActiveFocus());

    // Sanity check.
    QCOMPARE(canvas->window(), fileToolButton->window());
    QCOMPARE(canvas->splitter()->position(), 0.5);
    QVERIFY(!canvas->splitter()->isPressed());
    QVERIFY(!canvas->splitter()->isHovered());

    setupTempProjectDir();
}

void TestHelper::cleanup()
{
    switchMode(TileCanvas::PixelMode);
    switchTool(TileCanvas::PenTool);
    changeToolSize(1);

    canvas->setPenForegroundColour(QColor(Qt::black));
    canvas->setPenBackgroundColour(QColor(Qt::white));

    QVariant settingsAsVariant = qmlEngine(canvas)->rootContext()->contextProperty("settings");
    QVERIFY(settingsAsVariant.isValid());
    Settings *settings = settingsAsVariant.value<Settings*>();
    QVERIFY(settings);
    settings->resetShortcutsToDefaults();

    errorOccurredSpy.clear();
    tempProjectDir.reset();
    tempTilesetUrl.clear();
    cursorPos = QPoint();
    cursorWindowPos = QPoint();
}

void TestHelper::mouseEventOnCentre(QQuickItem *item, TestMouseEventType eventType)
{
    QQuickWindow *itemWindow = item->window();
    if (!itemWindow) {
        QString str;
        QDebug debug(&str);
        debug << "Item" << item << "has no window; can't send mouse events to it";
        QFAIL(qPrintable(str));
    }

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
    }
}

void TestHelper::mouseEvent(QQuickItem *item, const QPointF &localPos, TestMouseEventType eventType)
{
    const QPoint centre = item->mapToScene(localPos).toPoint();
    switch (eventType) {
    case MousePress:
        QTest::mousePress(item->window(), Qt::LeftButton, Qt::NoModifier, centre);
        break;
    case MouseRelease:
        QTest::mouseRelease(item->window(), Qt::LeftButton, Qt::NoModifier, centre);
        break;
    case MouseClick:
        QTest::mouseClick(item->window(), Qt::LeftButton, Qt::NoModifier, centre);
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

void TestHelper::changeCanvasSize(int width, int height)
{
    QCOMPARE(project->tilesWide(), 10);
    QCOMPARE(project->tilesHigh(), 10);

    // Open the canvas size popup.
    mouseEventOnCentre(canvasSizeButton, MouseClick);
    const QObject *canvasSizePopup = findPopupFromTypeName("CanvasSizePopup");
    QVERIFY(canvasSizePopup);
    QVERIFY(canvasSizePopup->property("visible").toBool());

    // Change the values and then cancel.
    // TODO: use actual input events...
    QQuickItem *widthSpinBox = canvasSizePopup->findChild<QQuickItem*>("changeCanvasWidthSpinBox");
    QVERIFY(widthSpinBox);
    widthSpinBox->setProperty("value", 11);

    QQuickItem *heightSpinBox = canvasSizePopup->findChild<QQuickItem*>("changeCanvasHeightSpinBox");
    QVERIFY(heightSpinBox);
    heightSpinBox->setProperty("value", 9);

    QQuickItem *cancelButton = canvasSizePopup->findChild<QQuickItem*>("canvasSizePopupCancelButton");
    QVERIFY(cancelButton);
    mouseEventOnCentre(cancelButton, MouseClick);
    QVERIFY(!canvasSizePopup->property("visible").toBool());
    QCOMPARE(project->tilesWide(), 10);
    QCOMPARE(project->tilesHigh(), 10);

    // Open the popup again.
    mouseEventOnCentre(canvasSizeButton, MouseClick);
    QVERIFY(canvasSizePopup);
    QVERIFY(canvasSizePopup->property("visible").toBool());
    // The old values should be restored.
    QCOMPARE(widthSpinBox->property("value").toInt(), 10);
    QCOMPARE(heightSpinBox->property("value").toInt(), 10);

    // Change the values and then press OK.
    QVERIFY(widthSpinBox->setProperty("value", width));
    QCOMPARE(widthSpinBox->property("value").toInt(), width);
    QVERIFY(heightSpinBox->setProperty("value", height));
    QCOMPARE(heightSpinBox->property("value").toInt(), height);

    QQuickItem *okButton = canvasSizePopup->findChild<QQuickItem*>("canvasSizePopupOkButton");
    QVERIFY(okButton);
    mouseEventOnCentre(okButton, MouseClick);
    QVERIFY(!canvasSizePopup->property("visible").toBool());
    QCOMPARE(project->tilesWide(), width);
    QCOMPARE(project->tilesHigh(), height);
    QCOMPARE(widthSpinBox->property("value").toInt(), width);
    QCOMPARE(heightSpinBox->property("value").toInt(), height);
}

void TestHelper::changeToolSize(int size)
{
    mouseEventOnCentre(toolSizeButton, MouseClick);
    const QObject *toolSizePopup = findPopupFromTypeName("ToolSizePopup");
    QVERIFY(toolSizePopup);
    QCOMPARE(toolSizePopup->property("visible").toBool(), true);

    QQuickItem *toolSizeSlider = toolSizePopup->findChild<QQuickItem*>("toolSizeSlider");
    QVERIFY(toolSizeSlider);

    // Slide the slider until we find the value we want.
    const QPoint sliderLeftEdge = toolSizeSlider->mapToScene(QPointF(0, toolSizeSlider->height() / 2)).toPoint();
    const QPoint sliderRightEdge = toolSizeSlider->mapToScene(QPointF(toolSizeSlider->width(), toolSizeSlider->height() / 2)).toPoint();
    QPoint sliderHandlePos = sliderLeftEdge;
    QTest::mousePress(toolSizeSlider->window(), Qt::LeftButton, Qt::NoModifier, sliderHandlePos);
    QCOMPARE(toolSizeSlider->property("pressed").toBool(), true);
    QCOMPARE(window->mouseGrabberItem(), toolSizeSlider);
    for (;
         int(sliderValue(toolSizeSlider)) != size && sliderHandlePos.x() < sliderRightEdge.x();
         ++sliderHandlePos.rx()) {
        QTest::mouseMove(toolSizeSlider->window(), sliderHandlePos, 5);
    }
    QTest::mouseRelease(toolSizeSlider->window(), Qt::LeftButton, Qt::NoModifier, sliderHandlePos);
    QCOMPARE(toolSizeSlider->property("pressed").toBool(), false);
    QCOMPARE(int(sliderValue(toolSizeSlider)), size);

    // Close the popup.
    QTest::keyClick(window, Qt::Key_Escape);
    QCOMPARE(toolSizePopup->property("visible").toBool(), false);
}

qreal TestHelper::sliderValue(QQuickItem *slider) const
{
    const qreal position = slider->property("position").toReal();
    qreal value = 0;
    QMetaObject::invokeMethod(slider, "valueAt", Qt::DirectConnection, Q_RETURN_ARG(qreal, value), Q_ARG(qreal, position));
    return value;
}

void TestHelper::drawPixelAtCursorPos()
{
    const Tile *targetTile = project->tileAt(cursorPos);
    QVERIFY(targetTile);

    switchTool(TileCanvas::PenTool);
    switchMode(TileCanvas::PixelMode);

    // Draw on some pixels of the current tile.
    const QImage originalTileImage = targetTile->tileset()->image()->copy(targetTile->sourceRect());
    QImage expectedImage = originalTileImage;
    expectedImage.setPixelColor(canvas->scenePosToTilePixelPos(cursorPos), canvas->penForegroundColour());

    QTest::mouseMove(window, cursorPos);
    QTest::mousePress(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
    QVERIFY(targetTile->tileset()->image()->copy(targetTile->sourceRect()) != originalTileImage);
    targetTile->tileset()->image()->copy(targetTile->sourceRect()).save("C:/Users/Mitch/AppData/Local/Temp/_actual.png");
    expectedImage.save("C:/Users/Mitch/AppData/Local/Temp/_expected.png");
    QCOMPARE(targetTile->tileset()->image()->copy(targetTile->sourceRect()), expectedImage);
    QVERIFY(project->hasUnsavedChanges());
    QVERIFY(window->title().contains("*"));

    QTest::mouseRelease(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
    QCOMPARE(targetTile->tileset()->image()->copy(targetTile->sourceRect()), expectedImage);
    QVERIFY(project->hasUnsavedChanges());
    QVERIFY(window->title().contains("*"));
}

void TestHelper::drawTileAtCursorPos()
{
    switchTool(TileCanvas::PenTool);
    switchMode(TileCanvas::TileMode);

    QTest::mouseMove(window, cursorWindowPos);
    QTest::mousePress(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
    QTest::mouseRelease(window, Qt::LeftButton, Qt::NoModifier, cursorWindowPos);
    const int penId = canvas->penTile()->id();
    QCOMPARE(project->tileAt(cursorPos)->id(), penId);
}

static QString fuzzyColourCompareFailMsg(const QColor &colour1, const QColor &colour2,
    const QChar &componentName, int difference, int fuzz)
{
    return QString::fromLatin1("colour1 %1 is not equal to colour2 %2; %3 difference of %4 is larger than fuzz of %5")
            .arg(colour1.name()).arg(colour2.name()).arg(componentName).arg(difference).arg(fuzz);
}

void TestHelper::fuzzyColourCompare(const QColor &colour1, const QColor &colour2, int fuzz)
{
    const int rDiff = qAbs(colour2.red() - colour1.red());
    QVERIFY2(rDiff <= fuzz, qPrintable(fuzzyColourCompareFailMsg(colour1, colour2, QLatin1Char('r'), rDiff, fuzz)));

    const int gDiff = qAbs(colour2.green() - colour1.green());
    QVERIFY2(gDiff <= fuzz, qPrintable(fuzzyColourCompareFailMsg(colour1, colour2, QLatin1Char('g'), gDiff, fuzz)));

    const int bDiff = qAbs(colour2.blue() - colour1.blue());
    QVERIFY2(bDiff <= fuzz, qPrintable(fuzzyColourCompareFailMsg(colour1, colour2, QLatin1Char('b'), bDiff, fuzz)));

    const int aDiff = qAbs(colour2.alpha() - colour1.alpha());
    QVERIFY2(aDiff <= fuzz, qPrintable(fuzzyColourCompareFailMsg(colour1, colour2, QLatin1Char('a'), aDiff, fuzz)));
}

void TestHelper::fuzzyImageCompare(const QImage &image1, const QImage &image2)
{
    QCOMPARE(image1.size(), image2.size());

    for (int y = 0; y < image1.height(); ++y) {
        for (int x = 0; x < image1.width(); ++x) {
            fuzzyColourCompare(image1.pixelColor(x, y), image2.pixelColor(x, y));
        }
    }
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

QPoint TestHelper::mapToTile(const QPoint &cursorPos) const
{
    return cursorPos - canvas->mapToScene(QPointF(0, 0)).toPoint();
}

QPoint TestHelper::tileSceneCentre(int xPosInTiles, int yPosInTiles) const
{
    return canvas->mapToScene(QPointF(
        xPosInTiles * project->tileWidth() + project->tileWidth() / 2,
        yPosInTiles * project->tileHeight() + project->tileHeight() / 2)).toPoint()
            + canvas->firstPane()->offset();
}

QPoint TestHelper::tileCanvasCentre(int xPosInTiles, int yPosInTiles) const
{
    return QPoint(
        xPosInTiles * project->tileWidth() + project->tileWidth() / 2,
        yPosInTiles * project->tileHeight() + project->tileHeight() / 2);
}

void TestHelper::setCursorPosInTiles(int xPosInTiles, int yPosInTiles)
{
    cursorPos = tileCanvasCentre(xPosInTiles, yPosInTiles);
    cursorWindowPos = tileSceneCentre(xPosInTiles, yPosInTiles);
}

void TestHelper::setCursorPosInPixels(int xPosInPixels, int yPosInPixels)
{
    cursorPos = QPoint(xPosInPixels, yPosInPixels);
    cursorWindowPos = canvas->mapToScene(cursorPos).toPoint() + canvas->firstPane()->offset();
    QVERIFY2(cursorWindowPos.x() >= 0 && cursorWindowPos.y() >= 0,
             qPrintable(QString::fromLatin1("x %1 y %2").arg(cursorWindowPos.x()).arg(cursorWindowPos.y())));
}

void TestHelper::setCursorPosInPixels(const QPoint &posInPixels)
{
    cursorPos = posInPixels;
    cursorWindowPos = canvas->mapToScene(QPointF(posInPixels.x(), posInPixels.y())).toPoint()
            + canvas->firstPane()->offset();
}

QPoint TestHelper::tilesetTileCentre(int xPosInTiles, int yPosInTiles) const
{
    return QPoint(
                xPosInTiles * project->tileWidth() + project->tileWidth() / 2,
                yPosInTiles * project->tileHeight() + project->tileHeight() / 2);
}

QPoint TestHelper::tilesetTileSceneCentre(int xPosInTiles, int yPosInTiles) const
{
    return tilesetSwatch->mapToScene(QPointF(
                                         xPosInTiles * project->tileWidth() + project->tileWidth() / 2,
                                         yPosInTiles * project->tileHeight() + project->tileHeight() / 2)).toPoint();
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

void TestHelper::createNewProject(int tileWidth, int tileHeight, int tilesetTilesWide, int tilesetTilesHigh)
{
    // Click the new project button.
    mouseEventOnCentre(fileToolButton, MouseClick);
    mouseEventOnCentre(newMenuButton, MouseClick);

    // Ensure that the new project popup is visible.
    const QObject *newProjectPopup = findPopupFromTypeName("NewProjectPopup");
    QVERIFY(newProjectPopup);
    QVERIFY(newProjectPopup->property("visible").toBool());
    // TODO: remove this when https://bugreports.qt.io/browse/QTBUG-53420 is fixed
    newProjectPopup->property("contentItem").value<QQuickItem*>()->forceActiveFocus();
    QVERIFY2(newProjectPopup->property("activeFocus").toBool(),
        qPrintable(QString::fromLatin1("NewProjectPopup doesn't have active focus (%1 does)").arg(window->activeFocusItem()->objectName())));

    // Ensure that the popup gets reset each time it's opened.
    QQuickItem *useExistingTilesetCheckBox = newProjectPopup->findChild<QQuickItem*>("useExistingTilesetCheckBox");
    QVERIFY(useExistingTilesetCheckBox);
    QVERIFY(useExistingTilesetCheckBox->property("checked").isValid());
    QVERIFY(!useExistingTilesetCheckBox->property("checked").toBool());

    QQuickItem *tilesetField = newProjectPopup->findChild<QQuickItem*>("tilesetPathTextField");
    QVERIFY(tilesetField);
    QVERIFY(tilesetField->property("text").isValid());
    QVERIFY(tilesetField->property("text").toString().isEmpty());

    QQuickItem *tileWidthSpinBox = newProjectPopup->findChild<QQuickItem*>("tileWidthSpinBox");
    QVERIFY(tileWidthSpinBox);
    QCOMPARE(tileWidthSpinBox->property("value").toInt(), 32);

    QQuickItem *tileHeightSpinBox = newProjectPopup->findChild<QQuickItem*>("tileHeightSpinBox");
    QVERIFY(tileHeightSpinBox);
    QCOMPARE(tileHeightSpinBox->property("value").toInt(), 32);

    QQuickItem *tilesWideSpinBox = newProjectPopup->findChild<QQuickItem*>("tilesWideSpinBox");
    QVERIFY(tilesWideSpinBox);
    QCOMPARE(tilesWideSpinBox->property("value").toInt(), 10);

    QQuickItem *tilesHighSpinBox = newProjectPopup->findChild<QQuickItem*>("tilesHighSpinBox");
    QVERIFY(tilesHighSpinBox);
    QCOMPARE(tilesHighSpinBox->property("value").toInt(), 10);

    QVERIFY(tileWidthSpinBox->setProperty("value", tileWidth));
    QCOMPARE(tileWidthSpinBox->property("value").toInt(), tileWidth);

    QVERIFY(tileHeightSpinBox->setProperty("value", tileHeight));
    QCOMPARE(tileHeightSpinBox->property("value").toInt(), tileHeight);

    if (tilesetTilesWide == -1 || tilesetTilesHigh == -1) {
        // Using an existing tileset image.

        // Check the "use existing tileset" checkbox.
        // Hack: it has focus in the app, but not in the test => meh
        useExistingTilesetCheckBox->forceActiveFocus();
        QVERIFY(useExistingTilesetCheckBox->hasActiveFocus());
        QTest::keyClick(window, Qt::Key_Space);
        QVERIFY(useExistingTilesetCheckBox->property("checked").toBool());

        // Input the filename.
        QTest::keyClick(window, Qt::Key_Tab);
        QVERIFY(tilesetField->hasActiveFocus());
        // TODO: input path using events
        QVERIFY(tilesetField->property("text").isValid());
        QVERIFY(tilesetField->setProperty("text", tempTilesetUrl.toString()));

        QQuickItem *invalidFileIcon = newProjectPopup->findChild<QQuickItem*>("invalidFileIcon");
        QVERIFY(invalidFileIcon);
        QObject *fileValidator = newProjectPopup->findChild<QObject*>("validator");
        QVERIFY2(qFuzzyCompare(invalidFileIcon->property("opacity").toReal(), 0),
            qPrintable(QString::fromLatin1("File should be valid, but got error message \"%1\"; file path: %2")
                .arg(fileValidator->property("errorMessage").toString())
                .arg(tilesetField->property("text").toString())));
    } else {
        // Using a new tileset image.
        QVERIFY(tilesetTilesWide > 0 && tilesetTilesHigh > 0);

        QVERIFY(tilesWideSpinBox->setProperty("value", tilesetTilesWide));
        QCOMPARE(tilesWideSpinBox->property("value").toInt(), tilesetTilesWide);

        QVERIFY(tilesHighSpinBox->setProperty("value", tilesetTilesHigh));
        QCOMPARE(tilesHighSpinBox->property("value").toInt(), tilesetTilesHigh);
    }

    // Confirm creation of the project.
    QQuickItem *okButton = newProjectPopup->findChild<QQuickItem*>("newProjectOkButton");
    QVERIFY(okButton);
    mouseEventOnCentre(okButton, MouseClick);
    QVERIFY(!newProjectPopup->property("visible").toBool());

    VERIFY_NO_ERRORS_OCCURRED();
    QCOMPARE(project->url(), QUrl());
    QVERIFY(project->hasLoaded());
    QVERIFY(project->isNewProject());
    QVERIFY(newMenuButton->isEnabled());
    QVERIFY(saveMenuButton->isEnabled());
    QVERIFY(saveAsMenuButton->isEnabled());
    QVERIFY(openMenuButton->isEnabled());
    QVERIFY(closeMenuButton->isEnabled());
    QVERIFY(settingsMenuButton->isEnabled());

    QVERIFY(imageGrabber.requestImage(canvas));
    QTRY_VERIFY(imageGrabber.isReady());
    QCOMPARE(imageGrabber.takeImage().pixelColor(16, 16), canvas->mapBackgroundColour());
}

void TestHelper::setupTempProjectDir()
{
    tempProjectDir.reset(new QTemporaryDir);
    QVERIFY2(tempProjectDir->isValid(), qPrintable(tempProjectDir->errorString()));

    QStringList toCopy;
    toCopy << tilesetBasename;
    // More stuff here.

    QStringList copiedPaths;

    foreach (const QString &basename, toCopy) {
        QFile sourceFile(":/resources/" + basename);
        QVERIFY2(sourceFile.open(QIODevice::ReadOnly), qPrintable(QString::fromLatin1(
            "Failed to open %1: %2").arg(sourceFile.fileName()).arg(sourceFile.errorString())));
        sourceFile.close();

        const QString saveFilePath = tempProjectDir->path() + "/" + basename;
        QVERIFY2(QFile::copy(sourceFile.fileName(), saveFilePath), qPrintable(QString::fromLatin1(
            "Failed to copy %1 to %2: %3").arg(sourceFile.fileName()).arg(saveFilePath).arg(sourceFile.errorString())));

        // A file copied from a file that is part of resources will always be read-only...
        QFile copiedFile(saveFilePath);
        QVERIFY(copiedFile.size() > 0);

        QVERIFY(copiedFile.setPermissions(QFile::ReadUser | QFile::WriteUser));
        QVERIFY(copiedFile.size() > 0);
        QVERIFY2(copiedFile.open(QIODevice::ReadWrite), qPrintable(QString::fromLatin1(
            "Error opening file at %1: %2").arg(saveFilePath).arg(copiedFile.errorString())));

        copiedPaths << saveFilePath;
    }

    tempTilesetUrl = QUrl::fromLocalFile(copiedPaths.at(0));
    // More stuff here.
}

void TestHelper::switchMode(TileCanvas::Mode mode)
{
    if (canvas->mode() == mode)
        return;

    mouseEventOnCentre(modeToolButton, MouseClick);
    const bool expectChecked = mode == TileCanvas::TileMode;
    QVERIFY2(modeToolButton->property("checked").toBool() == expectChecked, qPrintable(QString::fromLatin1(
        "modeToolButton.checked should be %1, but it's %2 (trying to set mode %3)")
            .arg(expectChecked).arg(modeToolButton->property("checked").toBool()).arg(mode)));
    QCOMPARE(canvas->mode(), mode);
}

void TestHelper::switchTool(TileCanvas::Tool tool)
{
    if (canvas->tool() == tool)
        return;

    if (tool == TileCanvas::PenTool) {
        mouseEventOnCentre(penToolButton, MouseClick);
    } else if (tool == TileCanvas::EyeDropperTool) {
        mouseEventOnCentre(eyeDropperToolButton, MouseClick);
    } else if (tool == TileCanvas::FillTool) {
        mouseEventOnCentre(fillToolButton, MouseClick);
    }
    QCOMPARE(canvas->tool(), tool);
}

void TestHelper::panTopLeftTo(int x, int y)
{
    const QPoint panDistance = QPoint(x, y) - canvas->firstPane()->offset();
    panBy(panDistance.x(), panDistance.y());
}

void TestHelper::panBy(int xDistance, int yDistance)
{
    QPoint pressPos = tileSceneCentre(4, 4);
    QTest::mouseMove(window, pressPos);
    QCOMPARE(canvas->currentPane(), canvas->firstPane());

    // TODO: get image checks working
    //        QVERIFY(imageGrabber.requestImage(canvas));
    //        QTRY_VERIFY(imageGrabber.isReady());
    //        const QImage originalImage = imageGrabber.takeImage();

    const QPoint originalOffset = canvas->currentPane()->offset();
    const QPoint expectedOffset = originalOffset + QPoint(xDistance, yDistance);

    QTest::keyPress(window, Qt::Key_Space);
    QCOMPARE(window->cursor().shape(), Qt::OpenHandCursor);
    QCOMPARE(canvas->currentPane()->offset(), originalOffset);
    //        QVERIFY(imageGrabber.requestImage(canvas));
    //        QTRY_VERIFY(imageGrabber.isReady());
    //        // Cursor changed to OpenHandCursor.
    //        QImage currentImage = imageGrabber.takeImage();
    //        QVERIFY(currentImage != originalImage);
    //        QImage lastImage = currentImage;

    QTest::mousePress(window, Qt::LeftButton, Qt::NoModifier, pressPos);
    QCOMPARE(window->cursor().shape(), Qt::ClosedHandCursor);
    QCOMPARE(canvas->currentPane()->offset(), originalOffset);
    //        QVERIFY(imageGrabber.requestImage(canvas));
    //        QTRY_VERIFY(imageGrabber.isReady());
    //        currentImage = imageGrabber.takeImage();
    //        // Cursor changed to ClosedHandCursor.
    //        QVERIFY(currentImage != lastImage);
    //        lastImage = currentImage;

    QTest::mouseMove(window, pressPos + QPoint(xDistance, yDistance));
    QCOMPARE(window->cursor().shape(), Qt::ClosedHandCursor);
    QCOMPARE(canvas->currentPane()->offset(), expectedOffset);
    //        QVERIFY(imageGrabber.requestImage(canvas));
    //        // Pane offset changed.
    //        currentImage = imageGrabber.takeImage();
    //        QVERIFY(currentImage != lastImage);
    //        lastImage = currentImage;

    QTest::mouseRelease(window, Qt::LeftButton, Qt::NoModifier, pressPos + QPoint(xDistance, yDistance));
    QCOMPARE(window->cursor().shape(), Qt::OpenHandCursor);
    QCOMPARE(canvas->currentPane()->offset(), expectedOffset);

    QTest::keyRelease(window, Qt::Key_Space);
    QCOMPARE(window->cursor().shape(), Qt::BlankCursor);
    QCOMPARE(canvas->currentPane()->offset(), expectedOffset);
}
