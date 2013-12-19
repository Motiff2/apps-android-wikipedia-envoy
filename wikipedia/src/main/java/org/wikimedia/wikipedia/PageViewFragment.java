package org.wikimedia.wikipedia;

import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.FrameLayout;
import android.widget.ProgressBar;
import android.widget.Toast;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.mediawiki.api.json.Api;
import org.wikimedia.wikipedia.events.PageStateChangeEvent;
import org.wikimedia.wikipedia.history.HistoryEntry;
import org.wikimedia.wikipedia.pageimages.PageImageSaveTask;
import org.wikimedia.wikipedia.savedpages.LoadSavedPageTask;
import org.wikimedia.wikipedia.savedpages.SavePageTask;

import java.util.ArrayList;
import java.util.List;

public class PageViewFragment extends Fragment {
    private static final String KEY_TITLE = "title";
    private static final String KEY_PAGE = "page";
    private static final String KEY_STATE = "state";
    private static final String KEY_SCROLL_Y = "scrollY";
    private static final String KEY_CURRENT_HISTORY_ENTRY = "currentHistoryEntry";

    public static final int STATE_NO_FETCH = 1;
    public static final int STATE_INITIAL_FETCH = 2;
    public static final int STATE_COMPLETE_FETCH = 3;

    private int state = STATE_NO_FETCH;

    private PageTitle title;
    private WebView webView;
    private ProgressBar loadProgress;
    private View networkError;
    private View retryButton;

    private Page page;
    private HistoryEntry curEntry;

    private CommunicationBridge bridge;
    private LinkHandler linkHandler;

    private WikipediaApp app;
    private Api api;

    private int scrollY;

    public PageViewFragment(PageTitle title, HistoryEntry historyEntry) {
        this.title = title;
        this.curEntry = historyEntry;
    }

    public PageViewFragment() {
    }

    public PageTitle getTitle() {
        return title;
    }

    public Page getPage() {
        return page;
    }

    private void displayLeadSection(Page page) {
        JSONObject leadSectionPayload = new JSONObject();
        try {
            leadSectionPayload.put("title", page.getTitle().getDisplayText());
            leadSectionPayload.put("leadSectionHTML", page.getSections().get(0).toHTML(true));
        } catch (JSONException e) {
            // This should never happen
            throw new RuntimeException(e);
        }

        bridge.sendMessage("displayLeadSection", leadSectionPayload);

        Utils.crossFade(loadProgress, webView);
    }

    private void populateNonLeadSections(Page page) {
        try {
            JSONObject wrapper = new JSONObject();
            JSONArray allSectionsPayload = new JSONArray();
            for (int i=1; i < page.getSections().size(); i++) {
                JSONObject sectionPayload = new JSONObject();
                sectionPayload.putOpt("index", i);
                sectionPayload.putOpt("heading", page.getSections().get(i).getHeading());
                sectionPayload.putOpt("content", page.getSections().get(i).toHTML(true));
                allSectionsPayload.put(sectionPayload);
            }
            wrapper.putOpt("sectionHeadings", allSectionsPayload);
            bridge.sendMessage("displaySectionsList", wrapper);
        } catch (JSONException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putParcelable(KEY_TITLE, title);
        outState.putParcelable(KEY_PAGE, page);
        outState.putInt(KEY_STATE, state);
        outState.putInt(KEY_SCROLL_Y, webView.getScrollY());
        outState.putParcelable(KEY_CURRENT_HISTORY_ENTRY, curEntry);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, final Bundle savedInstanceState) {
        FrameLayout parentView = (FrameLayout) inflater.inflate(R.layout.fragment_page, container, false);

        webView = (WebView) parentView.findViewById(R.id.pageWebView);
        loadProgress = (ProgressBar) parentView.findViewById(R.id.pageLoadProgress);
        networkError = parentView.findViewById(R.id.pageError);
        retryButton = parentView.findViewById(R.id.pageErrorRetry);

        if (savedInstanceState != null && savedInstanceState.containsKey(KEY_TITLE)) {
            title = savedInstanceState.getParcelable(KEY_TITLE);
            if (savedInstanceState.containsKey(KEY_PAGE)) {
                page = savedInstanceState.getParcelable(KEY_PAGE);
            }
            state = savedInstanceState.getInt(KEY_STATE);
            scrollY = savedInstanceState.getInt(KEY_SCROLL_Y);
            curEntry = savedInstanceState.getParcelable(KEY_CURRENT_HISTORY_ENTRY);
        }
        if (title == null) {
            throw new RuntimeException("No PageTitle passed in to constructor or in instanceState");
        }

        // Enable Pinch-Zoom
        webView.getSettings().setBuiltInZoomControls(true);
        webView.getSettings().setDisplayZoomControls(false);

        bridge = new CommunicationBridge(webView, "file:///android_asset/index.html");
        linkHandler = new LinkHandler(getActivity(), bridge, title.getSite());
        app = (WikipediaApp)getActivity().getApplicationContext();
        api = ((WikipediaApp)getActivity().getApplicationContext()).getAPIForSite(title.getSite());

        retryButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Utils.crossFade(networkError, loadProgress);
                performActionForState(state);
            }
        });

        setState(state);
        if (curEntry.getSource() == HistoryEntry.SOURCE_SAVED_PAGE && state < STATE_COMPLETE_FETCH) {
            new SavedPageFetchTask().execute();
        } else {
            performActionForState(state);
        }

        return parentView;
    }

    private void performActionForState(int state) {
        switch (state) {
            case STATE_NO_FETCH:
                new LeadSectionFetchTask().execute();
                break;
            case STATE_INITIAL_FETCH:
                new RestSectionsFetchTask().execute();
                break;
            case STATE_COMPLETE_FETCH:
                displayLeadSection(page);
                // Delay the full section population a little bit
                // To give the webview time to catch up.
                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        populateNonLeadSections(page);
                        webView.setScrollY(scrollY);
                    }
                }, 500);
                break;
        }
    }

    private void setState(int state) {
        this.state = state;
        app.getBus().post(new PageStateChangeEvent(state));
    }

    private class LeadSectionFetchTask extends SectionsFetchTask {
        public LeadSectionFetchTask() {
            super(api, title, "0");
        }

        @Override
        public void onFinish(List<Section> result) {
            page = new Page(title, (ArrayList<Section>) result);
            displayLeadSection(page);
            setState(STATE_INITIAL_FETCH);
            new RestSectionsFetchTask().execute();

            // Add history entry now
            app.getPersister(HistoryEntry.class).persist(curEntry);
            new PageImageSaveTask(app, api, title).execute();
        }

        @Override
        public void onCatch(Throwable caught) {
            // Should check for the source of the error and have different things turn up
            // But good enough for now
            Utils.crossFade(loadProgress, networkError);
            // Not sure why this is required, but without it tapping retry hides networkError
            // FIXME: INVESTIGATE WHY THIS HAPPENS!
            networkError.setVisibility(View.VISIBLE);
        }
    }

    private class RestSectionsFetchTask extends SectionsFetchTask {
        public RestSectionsFetchTask() {
            super(api, title, "1-");
        }

        @Override
        public void onFinish(List<Section> result) {
            ArrayList<Section> newSections = (ArrayList<Section>) page.getSections().clone();
            newSections.addAll(result);
            page = new Page(page.getTitle(), newSections);
            populateNonLeadSections(page);
            setState(STATE_COMPLETE_FETCH);
        }
    }

    private class SavedPageFetchTask extends LoadSavedPageTask {

        public SavedPageFetchTask() {
            super(getActivity(), title);
        }

        @Override
        public void onFinish(Page result) {
            page = result;
            performActionForState(STATE_COMPLETE_FETCH);
            setState(STATE_COMPLETE_FETCH);
        }
    }

    public void savePage() {
        new SavePageTask(getActivity(), bridge, page) {
            @Override
            public void onBeforeExecute() {
                Toast.makeText(getActivity(), R.string.toast_saving_page, Toast.LENGTH_LONG).show();
            }

            @Override
            public void onFinish(Void result) {
                Toast.makeText(getActivity(), R.string.toast_saved_page, Toast.LENGTH_LONG).show();
            }
        }.execute();
    }
}
