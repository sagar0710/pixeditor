/*
    Copyright 2020, Mitch Curtis

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

#include "imageproject.h"

#include "changeimagecanvassizecommand.h"
#include "changeimagesizecommand.h"
#include "utils.h"

ImageProject::ImageProject() :
    mAnimationHelper(this, &mAnimationSystem, &mUsingAnimation)
{
    setObjectName(QLatin1String("imageProject"));
    qCDebug(lcProjectLifecycle) << "constructing" << this;
}

ImageProject::~ImageProject()
{
    qCDebug(lcProjectLifecycle) << "destructing" << this;
}

QSize ImageProject::size() const
{
    return !mImage.isNull() ? mImage.size() : QSize();
}

void ImageProject::setSize(const QSize &newSize)
{
    if (newSize == size())
        return;

    beginMacro(QLatin1String("ChangeCanvasSize"));
    addChange(new ChangeImageCanvasSizeCommand(this, mImage, mImage.copy(0, 0, newSize.width(), newSize.height())));
    endMacro();
}

int ImageProject::widthInPixels() const
{
    return !mImage.isNull() ? mImage.width() : 0;
}

int ImageProject::heightInPixels() const
{
    return !mImage.isNull() ? mImage.height() : 0;
}

QRect ImageProject::bounds() const
{
    const QSize ourSize(size());
    return QRect(0, 0, ourSize.width(), ourSize.height());
}

void ImageProject::createNew(int imageWidth, int imageHeight, bool transparentBackground)
{
    if (hasLoaded()) {
        close();
    }

    if (mImageUrl.isEmpty()) {
        // Not using an existing image, so we must create one.
        mImageUrl = createTemporaryImage(imageWidth, imageHeight, transparentBackground ? Qt::transparent : Qt::white);
        if (!mImageUrl.isValid()) {
            return;
        }
    }

    qCDebug(lcProject) << "creating new project:"
        << "imageWidth =" << imageWidth
        << "imageHeight =" << imageHeight
        << "imageUrl =" << mImageUrl;

    Q_ASSERT(mUndoStack.count() == 0);
    Q_ASSERT(mImage.isNull());

    mImage = QImage(mImageUrl.toLocalFile());

    setUrl(QUrl());
    setNewProject(true);
    emit projectCreated();

    qCDebug(lcProject) << "finished creating new project";
}

void ImageProject::doLoad(const QUrl &url)
{
    mUsingTempImage = false;

    mImage = QImage(url.toLocalFile());
    if (mImage.isNull()) {
        error(QString::fromLatin1("Failed to open project's image at %1").arg(url.toLocalFile()));
        return;
    }

    setUrl(url);
    emit projectLoaded();
}

void ImageProject::doClose()
{
    mImage = QImage();
    mImageUrl = QUrl();
    mUsingTempImage = false;
    mUsingAnimation = false;
    mHasUsedAnimation = false;
    mAnimationSystem.reset();
    emit projectClosed();
}

bool ImageProject::doSaveAs(const QUrl &url)
{
    const QString filePath = url.toLocalFile();
    const QFileInfo projectSaveFileInfo(filePath);
    if (mTempDir.isValid()) {
        if (projectSaveFileInfo.dir().path() == mTempDir.path()) {
            error(QLatin1String("Cannot save project in internal temporary directory"));
            return false;
        }
    }

    if (mImage.isNull()) {
        error(QString::fromLatin1("Failed to save project: image is null"));
        return false;
    }

    if (!mImage.save(filePath)) {
        error(QString::fromLatin1("Failed to save project's image to %1").arg(filePath));
        return false;
    }

    mUsingTempImage = false;

    if (mFromNew) {
        // The project was successfully saved, so it can now save
        // to the same URL by default from now on.
        setNewProject(false);
    }
    setUrl(url);
    mUndoStack.setClean();
    mHadUnsavedChangesBeforeMacroBegan = false;
    return true;
}

void ImageProject::resize(int width, int height, bool smooth)
{
    const QSize newSize(width, height);
    if (newSize == size())
        return;

    beginMacro(QLatin1String("ChangeImageSize"));
    const QImage resized = mImage.scaled(newSize, Qt::IgnoreAspectRatio,
        smooth ? Qt::SmoothTransformation : Qt::FastTransformation);
    addChange(new ChangeImageSizeCommand(this, mImage, resized));
    endMacro();
}

void ImageProject::crop(const QRect &rect)
{
    if (rect.x() == 0 && rect.y() == 0 && rect.size() == size()) {
        // No change.
        return;
    }

    beginMacro(QLatin1String("CropImageCanvas"));
    const QImage resized = mImage.copy(rect);
    addChange(new ChangeImageCanvasSizeCommand(this, mImage, resized));
    endMacro();
}

QImage *ImageProject::image()
{
    return &mImage;
}

Project::Type ImageProject::type() const
{
    return ImageType;
}

void ImageProject::doSetCanvasSize(const QImage &newImage)
{
    doSetImageSize(newImage);
}

void ImageProject::doSetImageSize(const QImage &newImage)
{
    if (newImage.width() == 0 || newImage.height() == 0) {
        error(QLatin1String("Cannot set image's width or height to zero"));
        return;
    }

    Q_ASSERT(!mImage.isNull());
    Q_ASSERT(newImage.size() != size());
    mImage = newImage;
    emit sizeChanged();
}

AnimationSystem *ImageProject::animationSystem()
{
    return &mAnimationSystem;
}

void ImageProject::addAnimation()
{
    mAnimationHelper.addAnimation();
}

void ImageProject::duplicateAnimation(int index)
{
    mAnimationHelper.duplicateAnimation(index);
}

void ImageProject::modifyAnimation(int index)
{
    mAnimationHelper.modifyAnimation(index);
}

void ImageProject::renameAnimation(int index)
{
    mAnimationHelper.renameAnimation(index);
}

void ImageProject::moveCurrentAnimationUp()
{
    mAnimationHelper.moveCurrentAnimationUp();
}

void ImageProject::moveCurrentAnimationDown()
{
    mAnimationHelper.moveCurrentAnimationDown();
}

void ImageProject::removeAnimation(int index)
{
    mAnimationHelper.removeAnimation(index);
}

QImage ImageProject::exportedImage() const
{
    return mImage;
}

void ImageProject::exportGif(const QUrl &url)
{
    if (!mUsingAnimation) {
        // Shouldn't happen, but just in case...
        error(tr("Cannot export as GIF because the project isn't using animation"));
        return;
    }

    QString errorMessage;
    if (!Utils::exportGif(exportedImage(), url, *mAnimationSystem.currentAnimationPlayback(), errorMessage))
        error(errorMessage);
}

bool ImageProject::isUsingAnimation() const
{
    return mUsingAnimation;
}

void ImageProject::setUsingAnimation(bool usingAnimation)
{
    if (usingAnimation == mUsingAnimation)
        return;

    mUsingAnimation = usingAnimation;

    if (mUsingAnimation) {
        if (!mHasUsedAnimation) {
            mAnimationSystem.addNewAnimation(size());

            mHasUsedAnimation = true;
        }
    }

    emit usingAnimationChanged();
}
