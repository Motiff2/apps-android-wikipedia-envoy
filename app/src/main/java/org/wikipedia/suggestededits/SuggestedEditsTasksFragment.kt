package org.wikipedia.suggestededits

import android.content.Intent
import android.graphics.drawable.Drawable
import android.os.Bundle
import android.view.*
import android.view.View.GONE
import android.view.View.VISIBLE
import android.widget.LinearLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.content.res.AppCompatResources
import androidx.core.graphics.drawable.DrawableCompat
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers
import kotlinx.android.synthetic.main.fragment_suggested_edits_tasks.*
import org.wikipedia.Constants
import org.wikipedia.Constants.ACTIVITY_REQUEST_ADD_A_LANGUAGE
import org.wikipedia.Constants.InvokeSource.*
import org.wikipedia.R
import org.wikipedia.WikipediaApp
import org.wikipedia.auth.AccountUtil
import org.wikipedia.dataclient.Service
import org.wikipedia.dataclient.ServiceFactory
import org.wikipedia.dataclient.WikiSite
import org.wikipedia.language.LanguageSettingsInvokeSource
import org.wikipedia.settings.languages.WikipediaLanguagesActivity
import org.wikipedia.util.DimenUtil
import org.wikipedia.util.FeedbackUtil
import org.wikipedia.util.ResourceUtil
import org.wikipedia.util.log.L
import org.wikipedia.views.DefaultRecyclerAdapter
import org.wikipedia.views.DefaultViewHolder
import org.wikipedia.views.FooterMarginItemDecoration
import java.util.*
import kotlin.concurrent.schedule

class SuggestedEditsTasksFragment : Fragment() {
    private lateinit var addDescriptionsTask: SuggestedEditsTask
    private lateinit var addImageCaptionsTask: SuggestedEditsTask

    private val displayedTasks = ArrayList<SuggestedEditsTask>()
    private val callback = TaskViewCallback()

    private val disposables = CompositeDisposable()
    private val PADDING_16 = DimenUtil.roundedDpToPx(16.0f)
    private val PADDING_4 = DimenUtil.dpToPx(4.0f)

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        super.onCreateView(inflater, container, savedInstanceState)
        return inflater.inflate(R.layout.fragment_suggested_edits_tasks, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        (activity as AppCompatActivity).supportActionBar!!.elevation = 0f
        contributionsStatsView.setDescription("Contributions")
        contributionsStatsView.setImageDrawable(AppCompatResources.getDrawable(requireContext(), R.drawable.ic_mode_edit_white_24dp)!!)
        contributionsStatsView.setImageBackground(null)
        contributionsStatsView.setOnClickListener { onUserStatClicked(contributionsStatsView) }

        editStreakStatsView.setTitle("99")
        editStreakStatsView.setDescription("Edit streak")
        editStreakStatsView.setImageDrawable(AppCompatResources.getDrawable(requireContext(), R.drawable.ic_timer_black_24dp)!!)
        editStreakStatsView.setImageBackground(null)
        editStreakStatsView.setOnClickListener { onUserStatClicked(editStreakStatsView) }


        pageViewStatsView.setTitle("2984")
        pageViewStatsView.setDescription("Pageviews")
        pageViewStatsView.setImageDrawable(AppCompatResources.getDrawable(requireContext(), R.drawable.ic_trending_up_black_24dp)!!)
        pageViewStatsView.setImageBackground(null)
        pageViewStatsView.setOnClickListener { onUserStatClicked(pageViewStatsView) }


        editQualityStatsView.setTitle("Excellent")
        editQualityStatsView.setDescription("Edit quality")
        editQualityStatsView.setImageDrawable(AppCompatResources.getDrawable(requireContext(), R.drawable.ic_check_black_24dp)!!)
        editQualityStatsView.showCircularProgressBar(true)
        editQualityStatsView.setImageBackgroundTint(R.color.green90)
        editQualityStatsView.setImageParams(DimenUtil.roundedDpToPx(16.0f), DimenUtil.roundedDpToPx(16.0f))
        editQualityStatsView.setImageTint(ResourceUtil.getThemedAttributeId(context!!, R.attr.action_mode_green_background))
        editQualityStatsView.setOnClickListener { onUserStatClicked(editQualityStatsView) }

        swipeRefreshLayout.setColorSchemeResources(ResourceUtil.getThemedAttributeId(requireContext(), R.attr.colorAccent))
        swipeRefreshLayout.setOnRefreshListener { this.updateUI() }

        tasksRecyclerView.layoutManager = LinearLayoutManager(context, RecyclerView.VERTICAL, false)
        val topDecorationDp = 16
        tasksRecyclerView.addItemDecoration(FooterMarginItemDecoration(0, topDecorationDp))
        setUpTasks()
        tasksRecyclerView.adapter = RecyclerAdapter(displayedTasks)

        textViewForMessage.text = getString(R.string.suggested_edits_encouragement_message, AccountUtil.getUserName())

        //usernameText.text = AccountUtil.getUserName()
        /* userContributionsButton.setOnClickListener {
             startActivity(SuggestedEditsContributionsActivity.newIntent(requireContext()))
         }*/
    }

    private fun onUserStatClicked(view: View) {
        when (view) {
            contributionsStatsView -> showContributionsStatsViewTooltip()
            editStreakStatsView -> showEditStreakStatsViewTooltip()
            pageViewStatsView -> showPageViewStatsViewTooltip()
            else -> showEditQualityStatsViewTooltip()
        }
    }

    private fun showContributionsStatsViewTooltip() {
        tooltipLayout.visibility = VISIBLE
        val param = topTooltipArrow.layoutParams as LinearLayout.LayoutParams
        param.gravity = Gravity.START
        topTooltipArrow.layoutParams = param
        executeAfterTimer(true)
    }

    private fun executeAfterTimer(isTopTooltip: Boolean) {
        Timer("TooltipTimer", false).schedule(5000) {
            requireActivity().runOnUiThread(java.lang.Runnable {
                if (isTopTooltip) {
                    tooltipLayout.visibility = GONE
                } else {
                    bottomTooltipArrow.visibility = GONE
                    textViewForMessage.background = null
                    textViewForMessage.setPadding(0, 0, 0, 0)
                    textViewForMessage.elevation = 0.0f
                }
            })
        }
    }

    private fun showEditStreakStatsViewTooltip() {
        tooltipLayout.visibility = VISIBLE
        val param = topTooltipArrow.layoutParams as LinearLayout.LayoutParams
        param.gravity = Gravity.END
        topTooltipArrow.layoutParams = param
        executeAfterTimer(true)
    }

    private fun showPageViewStatsViewTooltip() {
        bottomTooltipArrow.visibility = VISIBLE
        val param = bottomTooltipArrow.layoutParams as LinearLayout.LayoutParams
        param.gravity = Gravity.START
        bottomTooltipArrow.layoutParams = param
        textViewForMessage.setBackgroundColor(ResourceUtil.getThemedColor(context!!, R.attr.paper_color))
        textViewForMessage.elevation = PADDING_4
        textViewForMessage.setPadding(PADDING_16, PADDING_16, PADDING_16, PADDING_16)
        executeAfterTimer(false)
    }

    private fun showEditQualityStatsViewTooltip() {
        bottomTooltipArrow.visibility = VISIBLE
        val param = bottomTooltipArrow.layoutParams as LinearLayout.LayoutParams
        param.gravity = Gravity.END
        bottomTooltipArrow.layoutParams = param
        textViewForMessage.setBackgroundColor(ResourceUtil.getThemedColor(context!!, R.attr.paper_color))
        textViewForMessage.elevation = PADDING_4
        textViewForMessage.setPadding(PADDING_16, PADDING_16, PADDING_16, PADDING_16)
        executeAfterTimer(false)
    }

    override fun onResume() {
        super.onResume()
        updateUI()
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        setHasOptionsMenu(true)
    }

    override fun onCreateOptionsMenu(menu: Menu, inflater: MenuInflater) {
        inflater.inflate(R.menu.menu_suggested_edits_tasks, menu)
        var drawable: Drawable = menu.findItem(R.id.menu_help).icon
        drawable = DrawableCompat.wrap(drawable)
        DrawableCompat.setTint(drawable, ResourceUtil.getThemedColor(context!!, R.attr.colorAccent))
        menu.findItem(R.id.menu_help).icon = drawable
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == ACTIVITY_REQUEST_ADD_A_LANGUAGE) {
            tasksRecyclerView.adapter!!.notifyDataSetChanged()
        }

    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.menu_help -> {
                FeedbackUtil.showAndroidAppEditingFAQ(requireContext())
                super.onOptionsItemSelected(item)
            }
            else -> super.onOptionsItemSelected(item)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        disposables.clear()
    }

    private fun fetchUserContributions() {
        disposables.add(ServiceFactory.get(WikiSite(Service.WIKIDATA_URL)).editorTaskCounts
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe({ response ->
                    val editorTaskCounts = response.query()!!.editorTaskCounts()!!
                    var totalEdits = 0
                    for (count in editorTaskCounts.descriptionEditsPerLanguage.values) {
                        totalEdits += count
                    }
                    for (count in editorTaskCounts.captionEditsPerLanguage.values) {
                        totalEdits += count
                    }
                    contributionsStatsView.setTitle(totalEdits.toString())
                }, { throwable ->
                    L.e(throwable)
                    FeedbackUtil.showError(requireActivity(), throwable)
                }))
    }

    private fun updateUI() {
        requireActivity().invalidateOptionsMenu()
        fetchUserContributions()
    }

    private fun setUpTasks() {
        displayedTasks.clear()
        addImageCaptionsTask = SuggestedEditsTask()
        addImageCaptionsTask.title = getString(R.string.suggested_edits_image_captions)
        addImageCaptionsTask.description = getString(R.string.suggested_edits_image_captions_task_detail)
        addImageCaptionsTask.imageDrawable = AppCompatResources.getDrawable(requireContext(), R.drawable.ic_icon_caption_images)
        displayedTasks.add(addImageCaptionsTask)

        addDescriptionsTask = SuggestedEditsTask()
        addDescriptionsTask.title = getString(R.string.description_edit_tutorial_title_descriptions)
        addDescriptionsTask.description = getString(R.string.suggested_edits_add_descriptions_task_detail)
        addDescriptionsTask.imageDrawable = AppCompatResources.getDrawable(requireContext(), R.drawable.ic_line_weight_black_24dp)
        displayedTasks.add(addDescriptionsTask)
    }


    private inner class TaskViewCallback : SuggestedEditsTaskView.Callback {
        override fun onViewClick(task: SuggestedEditsTask, isTranslate: Boolean) {
            if (WikipediaApp.getInstance().language().appLanguageCodes.size < Constants.MIN_LANGUAGES_TO_UNLOCK_TRANSLATION && isTranslate) {
                showLanguagesActivity(LanguageSettingsInvokeSource.SUGGESTED_EDITS.text())
                return
            }
            if (task == addDescriptionsTask) {
                startActivity(SuggestedEditsCardsActivity.newIntent(requireActivity(), if (isTranslate) SUGGESTED_EDITS_TRANSLATE_DESC else SUGGESTED_EDITS_ADD_DESC))
            } else if (task == addImageCaptionsTask) {
                startActivity(SuggestedEditsCardsActivity.newIntent(requireActivity(), if (isTranslate) SUGGESTED_EDITS_TRANSLATE_CAPTION else SUGGESTED_EDITS_ADD_CAPTION))
            }
        }
    }

    private fun showLanguagesActivity(invokeSource: String) {
        val intent = WikipediaLanguagesActivity.newIntent(requireActivity(), invokeSource)
        startActivityForResult(intent, ACTIVITY_REQUEST_ADD_A_LANGUAGE)
    }

    internal inner class RecyclerAdapter(tasks: List<SuggestedEditsTask>) : DefaultRecyclerAdapter<SuggestedEditsTask, SuggestedEditsTaskView>(tasks) {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): DefaultViewHolder<SuggestedEditsTaskView> {
            return DefaultViewHolder(SuggestedEditsTaskView(parent.context))
        }

        override fun onBindViewHolder(holder: DefaultViewHolder<SuggestedEditsTaskView>, i: Int) {
            holder.view.setUpViews(items()[i], callback)
        }
    }

    companion object {
        fun newInstance(): SuggestedEditsTasksFragment {
            return SuggestedEditsTasksFragment()
        }
    }
}
