#!/bin/bash
#
# generic pkg builder that clears all extra attributes after installation
#

APP_PATH=$1

[ "x" = "x$APP_PATH" ] && exit 1

PLUTIL="$(command -v plutil)"

[ "x" = "x$PLUTIL" ] && exit 2

PKGBUILD="$(command -v pkgbuild)"

[ "x" = "x$PKGBUILD" ] && exit 2

function pl_value() {
  $PLUTIL -extract $1 raw -o - "$APP_PATH/Contents/Info.plist"
}

BASE_NAME=$(basename "$APP_PATH" .app)
PKG_IDENTIFIER="$(pl_value CFBundleIdentifier)"
PKG_VERSION="$(pl_value CFBundleVersion)"
PKG_OUTPUT="$BASE_NAME.pkg"

[ -e scripts ] || mkdir scripts
[ -e scripts/postinstall ] || cat << EOF > scripts/postinstall
#!/bin/sh

xattr -cr "/Applications/$BASE_NAME.app"
EOF

chmod +x scripts/postinstall

$PKGBUILD \
  --identifier "$PKG_IDENTIFIER" \
  --version "$PKG_VERSION" \
  --scripts scripts/ \
  --install-location "/Applications/$BASE_NAME.app" \
  --root "$APP_PATH" \
  "$PKG_OUTPUT"
