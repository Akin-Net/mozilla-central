/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Download Manager UI Test Code.
 *
 * The Initial Developer of the Original Code is
 * Edward Lee <edward.lee@engineering.uiuc.edu>.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

/**
 * Test for bug 419403 that lets the download manager support multiple word
 * search against the download name, source/referrer, date/time, file size,
 * etc.
 */

function test()
{
  let dm = Cc["@mozilla.org/download-manager;1"].
           getService(Ci.nsIDownloadManager);
  let db = dm.DBConnection;

  // Empty any old downloads
  db.executeSimpleSQL("DELETE FROM moz_downloads");

  let stmt = db.createStatement(
    "INSERT INTO moz_downloads (name, target, source, state, endTime, maxBytes) " +
    "VALUES (?1, ?2, ?3, ?4, ?5, ?6)");

  try {
    for each (let site in ["ed.agadak.net", "mozilla.org"]) {
      stmt.bindStringParameter(0, "Super Pimped Download");
      stmt.bindStringParameter(1, "file://dummy/file");
      stmt.bindStringParameter(2, "http://" + site + "/file");
      stmt.bindInt32Parameter(3, dm.DOWNLOAD_FINISHED);
      stmt.bindInt64Parameter(4, new Date(1985, 7, 2) * 1000);
      stmt.bindInt64Parameter(5, 111222333444);

      // Add it!
      stmt.execute();
    }
  } finally {
    stmt.reset();
    stmt.finalize();
  }

  // Close the UI if necessary
  let wm = Cc["@mozilla.org/appshell/window-mediator;1"].
           getService(Ci.nsIWindowMediator);
  let win = wm.getMostRecentWindow("Download:Manager");
  if (win) win.close();

  let obs = Cc["@mozilla.org/observer-service;1"].
            getService(Ci.nsIObserverService);
  const DLMGR_UI_DONE = "download-manager-ui-done";

  let testPhase = 0;
  let testObs = {
    observe: function(aSubject, aTopic, aData) {
      if (aTopic != DLMGR_UI_DONE)
        return;

      let win = aSubject.QueryInterface(Ci.nsIDOMWindow);
      let $ = function(aId) win.document.getElementById(aId);
      let downloadView = $("downloadView");
      let searchbox = $("searchbox");

      let search = function(aTerms) {
        searchbox.value = aTerms;
        searchbox.doCommand();
      };

      // The list must have built, so figure out what test to do
      switch (testPhase++) {
        case 0:
          // Search for multiple words in any order in all places
          search("download super pimped 104 GB august 2");

          break;
        case 1:
          // Done populating the two items
          ok(downloadView.itemCount == 2, "Search matched both downloads");

          // Do partial word matches including the site
          search("agadak aug 2 downl 104");

          break;
        case 2:
          // Done populating the one result
          ok(downloadView.itemCount == 1, "Found the single download");

          // We're done!
          obs.removeObserver(testObs, DLMGR_UI_DONE);
          finish();

          break;
      }
    }
  };
  obs.addObserver(testObs, DLMGR_UI_DONE, false);
 
  // Show the Download Manager UI
  Cc["@mozilla.org/download-manager-ui;1"].
  getService(Ci.nsIDownloadManagerUI).show();

  waitForExplicitFinish();
}
