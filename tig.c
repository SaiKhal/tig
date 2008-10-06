#ifdef HAVE_NCURSESW_NCURSES_H
#include <ncursesw/ncurses.h>
#else
#ifdef HAVE_NCURSES_NCURSES_H
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#endif
static size_t utf8_length(const char *string, int *width, size_t max_width, int *trimmed, bool reserve);
static bool prompt_yesno(const char *prompt);
#define SIZEOF_REV	41	/* Holds a SHA-1 and an ending NUL. */
#define SIZEOF_ARG	32	/* Default argument array size. */
#define TAB_SIZE	8
#define GIT_CONFIG "config"
	"git ls-remote . 2>/dev/null"
#define TIG_MAIN_BASE \
	"git log --no-color --pretty=raw --parents --topo-order"

	TIG_MAIN_BASE " %s 2>/dev/null"
	REQ_(STATUS_CHECKOUT,	"Checkout file"), \
	REQ_(STAGE_NEXT,	"Find next chunk to stage"), \
static bool opt_line_graphics		= TRUE;
static int opt_tab_size			= TAB_SIZE;
static int opt_author_cols		= AUTHOR_COLS-1;
static enum request
	enum request request = REQ_VIEW_MAIN;
		return REQ_VIEW_PAGER;
		return REQ_VIEW_MAIN;
		return REQ_VIEW_STATUS;
		return REQ_VIEW_BLAME;
		request = REQ_VIEW_DIFF;
		request = subcommand[0] == 'l' ? REQ_VIEW_LOG : REQ_VIEW_DIFF;
		string_copy(opt_cmd, TIG_MAIN_BASE);
			return REQ_NONE;
			return REQ_NONE;
	return request;
	LINE_INFO,
	LINE_NONE
get_line_type(const char *line)
get_line_info(const char *name)
	{ KEY_F(5),	REQ_REFRESH },
	{ '!',		REQ_STATUS_CHECKOUT },
	{ '@',		REQ_STAGE_NEXT },
add_run_request(enum keymap keymap, int key, int argc, const char **argv)
	struct run_request *req;
	char cmd[SIZEOF_STR];
		if (!string_format_from(cmd, &bufpos, "%s ", *argv))
	req = realloc(run_request, (run_requests + 1) * sizeof(*run_request));
	if (!req)
	run_request = req;
	req = &run_request[run_requests++];
	string_copy(req->cmd, cmd);
	req->keymap = keymap;
	req->key = key;
		const char *argv[1];
option_color_command(int argc, const char *argv[])
static int
parse_int(const char *s, int default_value, int min, int max)
{
	int value = atoi(s);

	return (value < min || value > max) ? default_value : value;
}

option_set_command(int argc, const char *argv[])
	if (!strcmp(argv[0], "line-graphics")) {
		opt_line_graphics = parse_bool(argv[2]);
		return OK;
	}

		opt_num_interval = parse_int(argv[2], opt_num_interval, 1, 1024);
		return OK;
	}

	if (!strcmp(argv[0], "author-width")) {
		opt_author_cols = parse_int(argv[2], opt_author_cols, 0, 1024);
		opt_tab_size = parse_int(argv[2], opt_tab_size, 1, 1024);
		const char *arg = argv[2];
		int arglen = strlen(arg);
		switch (arg[0]) {
			if (arglen == 1 || arg[arglen - 1] != arg[0]) {
				config_msg = "Unmatched quotation";
				return ERR;
			}
			arg += 1; arglen -= 2;
option_bind_command(int argc, const char *argv[])
	const char *argv[SIZEOF_ARG];
	/* Drawing */
	struct line *curline;	/* Line currently being drawn. */
	enum line_type curtype;	/* Attribute currently used for drawing. */
	unsigned long col;	/* Column when drawing. */

	bool (*draw)(struct view *view, struct line *line, unsigned int lineno);

enum line_graphic {
	LINE_GRAPHIC_VLINE
};

static int line_graphics[] = {
	/* LINE_GRAPHIC_VLINE: */ '|'
};

static inline void
set_view_attr(struct view *view, enum line_type type)
{
	if (!view->curline->selected && view->curtype != type) {
		wattrset(view->win, get_line_attr(type));
		wchgat(view->win, -1, 0, type, NULL);
		view->curtype = type;
	}
}

draw_chars(struct view *view, enum line_type type, const char *string,
	   int max_len, bool use_tilde)
	int col = 0;
		len = utf8_length(string, &col, max_len, &trimmed, use_tilde);
		col = len = strlen(string);
			col = len = max_len;
	set_view_attr(view, type);
		set_view_attr(view, LINE_DELIMITER);
		col++;
	return col;
draw_space(struct view *view, enum line_type type, int max, int spaces)
{
	static char space[] = "                    ";
	int col = 0;

	spaces = MIN(max, spaces);

	while (spaces > 0) {
		int len = MIN(spaces, sizeof(space) - 1);

		col += draw_chars(view, type, space, spaces, FALSE);
		spaces -= len;
	}

	return col;
}

static bool
draw_lineno(struct view *view, unsigned int lineno)
	char number[10];
	int max = view->width - view->col;
	if (max < max_number)
		max_number = max;

		static char fmt[] = "%1ld";

		col = draw_chars(view, LINE_LINE_NUMBER, number, max_number, TRUE);
	} else {
		col = draw_space(view, LINE_LINE_NUMBER, max_number, max_number);
		set_view_attr(view, LINE_DEFAULT);
		waddch(view->win, line_graphics[LINE_GRAPHIC_VLINE]);

	if (col < max)
		col += draw_space(view, LINE_DEFAULT, max - col, 1);
	view->col += col;

	return view->width - view->col <= 0;
}

static bool
draw_text(struct view *view, enum line_type type, const char *string, bool trim)
{
	view->col += draw_chars(view, type, string, view->width - view->col, trim);
	return view->width - view->col <= 0;
}

static bool
draw_graphic(struct view *view, enum line_type type, chtype graphic[], size_t size)
{
	int max = view->width - view->col;
	int i;

	if (max < size)
		size = max;

	set_view_attr(view, type);
	/* Using waddch() instead of waddnstr() ensures that
	 * they'll be rendered correctly for the cursor line. */
	for (i = 0; i < size; i++)
		waddch(view->win, graphic[i]);

	view->col += size;
	if (size < max) {
		view->col++;
	return view->width - view->col <= 0;
static bool
draw_field(struct view *view, enum line_type type, char *text, int len, bool trim)
	int max = MIN(view->width - view->col, len);

	if (text)
		col = draw_chars(view, type, text, max - 1, trim);
	else
		col = draw_space(view, type, max - 1, max - 1);

	view->col += col + draw_space(view, LINE_DEFAULT, max - col, max - col);
	return view->width - view->col <= 0;
}

static bool
draw_date(struct view *view, struct tm *time)
{
	char buf[DATE_COLS];
	char *date;
	date = timelen ? buf : NULL;
	return draw_field(view, LINE_DATE, date, DATE_COLS, FALSE);
	view->col = 0;
	view->curline = line;
	view->curtype = LINE_NONE;
	line->selected = FALSE;
		set_view_attr(view, LINE_CURSOR);
	draw_ok = view->ops->draw(view, line, lineno);
end_update(struct view *view, bool force)
	while (!view->ops->read(view, NULL))
		if (!force)
			return;
	if (ferror(view->pipe) && errno != 0) {
		end_update(view, TRUE);
		end_update(view, FALSE);
	end_update(view, TRUE);
	OPEN_NOMAXIMIZE = 8	/* Do not maximize the current view. */
	if (view->pipe)
		end_update(view, TRUE);

static bool
run_confirm(const char *cmd, const char *prompt)
{
	bool confirmation = prompt_yesno(prompt);

	if (confirmation)
		system(cmd);

	return confirmation;
}

		    view == VIEW(REQ_VIEW_MAIN) ||
			end_update(view, TRUE);
pager_draw(struct view *view, struct line *line, unsigned int lineno)
	if (opt_line_number && draw_lineno(view, lineno))
		return TRUE;
	draw_text(view, line->type, text, TRUE);
blame_draw(struct view *view, struct line *line, unsigned int lineno)
	struct tm *time = NULL;
	char *id = NULL, *author = NULL;
	if (blame->commit && *blame->commit->filename) {
		id = blame->commit->id;
		author = blame->commit->author;
		time = &blame->commit->time;
	if (opt_date && draw_date(view, time))
		return TRUE;
	if (opt_author &&
	    draw_field(view, LINE_MAIN_AUTHOR, author, opt_author_cols, TRUE))
		return TRUE;
	if (draw_field(view, LINE_BLAME_ID, id, ID_COLS, FALSE))
		return TRUE;
	if (draw_lineno(view, lineno))
	draw_text(view, LINE_DEFAULT, blame->text, TRUE);
#define MATCH(text, on)							\
	(on && *text && regexec(view->regex, text, 1, &pmatch, 0) != REG_NOMATCH)
		if (MATCH(commit->title, 1) ||
		    MATCH(commit->author, opt_author) ||
		    MATCH(commit->id, opt_date))
		    MATCH(buf, 1))
	return MATCH(blame->text, 1);
static size_t stage_chunks;
static int *stage_chunk;

/* This should work even for the "On branch" line. */
static inline bool
status_has_none(struct view *view, struct line *line)
{
	return line < view->line + view->lines && !line[1].data;
}
	"git ls-files -z --others --exclude-standard"
	"git ls-files -z --cached --exclude-standard"
	system("git update-index -q --refresh >/dev/null 2>/dev/null");
	if (opt_no_head &&
	    !status_run(view, STATUS_LIST_NO_HEAD_CMD, 'A', LINE_STAT_STAGED))
		return FALSE;
	else if (!status_run(view, STATUS_DIFF_INDEX_CMD, 0, LINE_STAT_STAGED))
	if (!status_run(view, STATUS_DIFF_FILES_CMD, 0, LINE_STAT_UNSTAGED) ||
	    !status_run(view, STATUS_LIST_OTHER_CMD, '?', LINE_STAT_UNTRACKED))
status_draw(struct view *view, struct line *line, unsigned int lineno)
	enum line_type type;
			type = LINE_STAT_SECTION;
			type = LINE_STAT_SECTION;
			type = LINE_STAT_SECTION;
			type = LINE_DEFAULT;
			type = LINE_STAT_HEAD;
		static char buf[] = { '?', ' ', ' ', ' ', 0 };
		buf[0] = status->status;
		if (draw_text(view, line->type, buf, TRUE))
			return TRUE;
		type = LINE_DEFAULT;
	draw_text(view, type, text, TRUE);
		stage_chunks = 0;
		if (!status_update_files(view, line + 1)) {
			return FALSE;
		}
		return FALSE;
static bool
status_checkout(struct status *status, enum line_type type, bool has_next)
{
	if (!status || type != LINE_STAT_UNSTAGED) {
		if (has_next) {
			report("Nothing to checkout");
		} else if (type == LINE_STAT_UNTRACKED) {
			report("Cannot checkout untracked files");
		} else if (type == LINE_STAT_STAGED) {
			report("Cannot checkout staged files");
		} else {
			report("Cannot checkout multiple files");
		}
		return FALSE;

	} else {
		char cmd[SIZEOF_STR];
		char file_sq[SIZEOF_STR];

		if (sq_quote(file_sq, 0, status->old.name) >= sizeof(file_sq) ||
		    !string_format(cmd, "git checkout %s%s", opt_cdup, file_sq))
			return FALSE;

		return run_confirm(cmd, "Are you sure you want to overwrite any changes?");
	}
}

	case REQ_STATUS_CHECKOUT:
		if (!status_checkout(status, line->type, status_has_none(view, line)))
			return REQ_NONE;
		break;

	} else if (!stage_status.status) {
		view = VIEW(REQ_VIEW_STATUS);

		for (line = view->line; line < view->line + view->lines; line++)
			if (line->type == stage_line_type)
				break;

		if (!status_update_files(view, line + 1)) {
			report("Failed to update files");
			return FALSE;
		}

static void
stage_next(struct view *view, struct line *line)
{
	int i;

	if (!stage_chunks) {
		static size_t alloc = 0;
		int *tmp;

		for (line = view->line; line < view->line + view->lines; line++) {
			if (line->type != LINE_DIFF_CHUNK)
				continue;

			tmp = realloc_items(stage_chunk, &alloc,
					    stage_chunks, sizeof(*tmp));
			if (!tmp) {
				report("Allocation failure");
				return;
			}

			stage_chunk = tmp;
			stage_chunk[stage_chunks++] = line - view->line;
		}
	}

	for (i = 0; i < stage_chunks; i++) {
		if (stage_chunk[i] > view->lineno) {
			do_scroll_view(view, stage_chunk[i] - view->lineno);
			report("Chunk %d of %d", i + 1, stage_chunks);
			return;
		}
	}

	report("No next chunk found");
}

		if (!stage_update(view, line))
			return REQ_NONE;
		break;

	case REQ_STATUS_CHECKOUT:
		if (!status_checkout(&stage_status, stage_line_type, FALSE))
			return REQ_NONE;
	case REQ_STAGE_NEXT:
		if (stage_line_type == LINE_STAT_UNTRACKED) {
			report("File is untracked; press %s to add",
			       get_key(REQ_STATUS_UPDATE));
			return REQ_NONE;
		}
		stage_next(view, line);
		return REQ_NONE;

static void
clear_rev_graph(struct rev_graph *graph)
{
	graph->boundary = 0;
	graph->size = graph->pos = 0;
	graph->commit = NULL;
	memset(graph->parents, 0, sizeof(*graph->parents));
}

	clear_rev_graph(graph);
		{ ' ',	'|' },
	if (opt_line_graphics)
		fillers[DEFAULT].line = line_graphics[LINE_GRAPHIC_VLINE];

static int load_refs(void);

main_draw(struct view *view, struct line *line, unsigned int lineno)
	if (opt_date && draw_date(view, &commit->time))
		return TRUE;
	if (opt_author &&
	    draw_field(view, LINE_MAIN_AUTHOR, commit->author, opt_author_cols, TRUE))
		return TRUE;
	if (opt_rev_graph && commit->graph_size &&
	    draw_graphic(view, LINE_MAIN_REVGRAPH, commit->graph, commit->graph_size))
		return TRUE;
			enum line_type type;

			if (commit->refs[i]->head)
				type = LINE_MAIN_HEAD;
				type = LINE_MAIN_LOCAL_TAG;
				type = LINE_MAIN_TAG;
				type = LINE_MAIN_TRACKED;
				type = LINE_MAIN_REMOTE;
				type = LINE_MAIN_REF;

			if (draw_text(view, type, "[", TRUE) ||
			    draw_text(view, type, commit->refs[i]->name, TRUE) ||
			    draw_text(view, type, "]", TRUE))
				return TRUE;

			if (draw_text(view, LINE_DEFAULT, " ", TRUE))
	draw_text(view, LINE_DEFAULT, commit->title, TRUE);
		int i;

		if (view->lines > 0) {
			commit = view->line[view->lines - 1].data;
			if (!*commit->author) {
				view->lines--;
				free(commit);
				graph->commit = NULL;
			}
		}

		for (i = 0; i < ARRAY_SIZE(graph_stacks); i++)
			clear_rev_graph(&graph_stacks[i]);
	switch (request) {
	case REQ_ENTER:
		break;
	case REQ_REFRESH:
		load_refs();
		string_copy(opt_cmd, view->cmd);
		open_view(view, REQ_VIEW_MAIN, OPEN_RELOAD);
		break;
	default:
	}
static bool
grep_refs(struct ref **refs, regex_t *regex)
{
	regmatch_t pmatch;
	size_t i = 0;

	if (!refs)
		return FALSE;
	do {
		if (regexec(regex, refs[i]->name, 1, &pmatch, 0) != REG_NOMATCH)
			return TRUE;
	} while (refs[i++]->next);

	return FALSE;
}

	enum { S_TITLE, S_AUTHOR, S_DATE, S_REFS, S_END } state;
		case S_AUTHOR:
			if (!opt_author)
				continue;
			text = commit->author;
			break;
			if (!opt_date)
				continue;
		case S_REFS:
			if (!opt_show_refs)
				continue;
			if (grep_refs(commit->refs, view->regex) == TRUE)
				return TRUE;
			continue;
utf8_length(const char *string, int *width, size_t max_width, int *trimmed, bool reserve)
	size_t last_ucwidth = 0;
	*width = 0;
		*width  += ucwidth;
		if (*width > max_width) {
			*width -= ucwidth;
			if (reserve && *width == max_width) {
				*width -= last_ucwidth;
		last_ucwidth = ucwidth;

	TABSIZE = opt_tab_size;
	if (opt_line_graphics) {
		line_graphics[LINE_GRAPHIC_VLINE] = ACS_VLINE;
	}
}

static bool
prompt_yesno(const char *prompt)
{
	enum { WAIT, STOP, CANCEL  } status = WAIT;
	bool answer = FALSE;

	while (status == WAIT) {
		struct view *view;
		int i, key;

		input_mode = TRUE;

		foreach_view (view, i)
			update_view(view);

		input_mode = FALSE;

		mvwprintw(status_win, 0, 0, "%s [Yy]/[Nn]", prompt);
		wclrtoeol(status_win);

		/* Refresh, accept single keystroke of input */
		key = wgetch(status_win);
		switch (key) {
		case ERR:
			break;

		case 'y':
		case 'Y':
			answer = TRUE;
			status = STOP;
			break;

		case KEY_ESC:
		case KEY_RETURN:
		case KEY_ENTER:
		case KEY_BACKSPACE:
		case 'n':
		case 'N':
		case '\n':
		default:
			answer = FALSE;
			status = CANCEL;
		}
	}

	/* Clear the status window */
	status_empty = FALSE;
	report("");

	return answer;
	if (!*opt_git_dir)
		return OK;

	while (refs_size > 0)
		free(refs[--refs_size].name);
	while (id_refs_size > 0)
		free(id_refs[--id_refs_size]);

	return read_properties(popen("git " GIT_CONFIG " --list", "r"),
	request = parse_options(argc, argv);
	if (request == REQ_NONE)
	if (!opt_git_dir[0] && request != REQ_VIEW_PAGER)
	if (load_refs() == ERR)
					request = REQ_VIEW_DIFF;
					request = REQ_VIEW_PAGER;

				/* Always reload^Wrerun commands from the prompt. */
				open_view(view, request, OPEN_RELOAD);