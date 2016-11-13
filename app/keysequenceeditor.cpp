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

#include "keysequenceeditor.h"

#include <QKeyEvent>
#include <QLoggingCategory>
#include <QMetaEnum>

Q_LOGGING_CATEGORY(lcKeySequenceEditor, "app.keySequenceEditor")

KeySequenceEditor::KeySequenceEditor() :
    mCurrentKeyIndex(0)
{
    mKeysPressed.fill(0, 4);
}

QString KeySequenceEditor::originalSequence() const
{
    return mOriginalSequence.toString();
}

void KeySequenceEditor::setOriginalSequence(const QString &originalSequence)
{
    if (originalSequence == mOriginalSequence.toString())
        return;

    mOriginalSequence = QKeySequence(originalSequence, QKeySequence::PortableText);
    setCurrentSequence(QString());
    setNewSequence(QString());
    emit originalSequenceChanged();
    // This might not always be the case, I'm just lazy.
    emit hasChangedChanged();
    emit displaySequenceChanged();
}

QString KeySequenceEditor::newSequence() const
{
    return mNewSequence.toString();
}

QString KeySequenceEditor::displaySequence() const
{
    QKeySequence sequence = hasActiveFocus()
            ? mCurrentSequence : (!mNewSequence.isEmpty() ? mNewSequence : mOriginalSequence);
    return sequence.toString();
}

void KeySequenceEditor::setNewSequence(const QString &newSequence)
{
    if (newSequence == mNewSequence.toString())
        return;

    mNewSequence = QKeySequence(newSequence, QKeySequence::PortableText);
    qDebug(lcKeySequenceEditor) << "Set new sequence to" << mNewSequence;
    emit newSequenceChanged();
    emit hasChangedChanged();
    emit displaySequenceChanged();
}

void KeySequenceEditor::resetKeysPressed()
{
    qDebug(lcKeySequenceEditor) << "Clearing pressed keys";

    for (int i = 0; i < mKeysPressed.size(); ++i) {
        mKeysPressed[i] = 0;
    }

    mCurrentKeyIndex = 0;
}

bool KeySequenceEditor::hasChanged() const
{
    return !mNewSequence.isEmpty() && mNewSequence != mOriginalSequence;
}

void KeySequenceEditor::reset()
{
    setCurrentSequence(originalSequence());
    setNewSequence(originalSequence());
    resetKeysPressed();
}

void KeySequenceEditor::setCurrentSequence(const QString &currentSequence)
{
    if (currentSequence == mCurrentSequence.toString())
        return;

    mCurrentSequence = QKeySequence(currentSequence, QKeySequence::PortableText);
    qDebug(lcKeySequenceEditor) << "Current sequence changed to" << mCurrentSequence;
    emit hasChangedChanged();
    emit displaySequenceChanged();
}

class KeyHelper : private QObject {
public:
    static QString keyName(int index) {
        static int keyEnumIndex = staticQtMetaObject.indexOfEnumerator("Key");
        return staticQtMetaObject.enumerator(keyEnumIndex).valueToKey(index);
    }
};

void KeySequenceEditor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        setFocus(false);
    } else if (event->key() == Qt::Key_Return) {
        accept();
    } else if (!event->isAutoRepeat()) {
        int modifiers = 0;
        if (event->modifiers().testFlag(Qt::ControlModifier))
            modifiers |= Qt::CTRL;
        if (event->modifiers().testFlag(Qt::ShiftModifier))
            modifiers |= Qt::SHIFT;
        if (event->modifiers().testFlag(Qt::AltModifier))
            modifiers |= Qt::ALT;
        if (event->modifiers().testFlag(Qt::MetaModifier))
            modifiers |= Qt::META;

        if (event->key() >= Qt::Key_Shift && event->key() <= Qt::Key_Meta) {
            qDebug(lcKeySequenceEditor) << "Only modifiers were pressed ("
                << event->text() << "/" << KeyHelper::keyName(event->key()) << "/" << QKeySequence(event->key()) << "; ignoring";
        } else {
            mKeysPressed[mCurrentKeyIndex++] = event->key() | modifiers;
            qDebug(lcKeySequenceEditor) << "Adding key" << event->text() << "/" << KeyHelper::keyName(event->key()) << "/"
                    << QKeySequence(event->key()) << " with modifiers" << modifiers
                    << "(" << QKeySequence(mKeysPressed[mCurrentKeyIndex - 1]) << ") to pressed keys";

            const QKeySequence sequence(mKeysPressed[0], mKeysPressed[1], mKeysPressed[2], mKeysPressed[3]);
            setCurrentSequence(sequence.toString());

            if (mCurrentKeyIndex == mKeysPressed.size() - 1) {
                // That was the last key out of four possible keys; end it here.
                accept();
            }
        }
    }

    event->accept();
}

void KeySequenceEditor::keyReleaseEvent(QKeyEvent *event)
{
    event->accept();
}

void KeySequenceEditor::focusInEvent(QFocusEvent *event)
{
    event->accept();

    // The text displaying the shortcut should be cleared when editing begins.
    emit displaySequenceChanged();
}

void KeySequenceEditor::focusOutEvent(QFocusEvent *event)
{
    event->accept();

    cancel();
}

void KeySequenceEditor::accept()
{
    qDebug(lcKeySequenceEditor) << "Attempting to accept input...";

    // If there hasn't been anything new successfully entered yet, check against
    // the original sequence, otherwise check against the latest successfully entered sequence.
    // Note: hasChanged() assumes that an empty sequence isn't possible; we might want to accunt for this
    // in the future.
    if ((mCurrentSequence != mOriginalSequence) || (hasChanged() && mCurrentSequence != mNewSequence)) {
        if (validate(mCurrentSequence)) {
            qDebug(lcKeySequenceEditor) << "Input valid";
            setNewSequence(mCurrentSequence.toString());
        } else {
            qDebug(lcKeySequenceEditor) << "Input invalid";
            cancel();
        }
    } else {
        qDebug(lcKeySequenceEditor) << "Nothing has changed in the input";
        // Nothing's changed.
    }

    resetKeysPressed();
    setFocus(false);
}

void KeySequenceEditor::cancel()
{
    resetKeysPressed();
    setCurrentSequence(QString());
}

bool KeySequenceEditor::validate(const QKeySequence &sequence) const
{
    qDebug(lcKeySequenceEditor) << "Validating key sequence" << sequence << "...";

    bool valid = true;//false;

    // do some checks

    return valid;
}
