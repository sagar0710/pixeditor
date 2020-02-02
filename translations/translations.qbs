import qbs

Product {
    name: "translations"
    type: "qm"
    files: "*.ts"

    Depends { name: "Qt.core" }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: {
            if (qbs.targetOS.contains("windows") || project.linuxArchive)
                return "translations"
            else if (qbs.targetOS.contains("macos"))
                return "Slate.app/Contents/Translations"
            else
                return "share/slate/translations"
        }
    }
}
