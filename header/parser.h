/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:54:15 by francesca         #+#    #+#             */
/*   Updated: 2025/07/17 15:43:38 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>

// #define INITIAL_CAPACITY 16
// ==============================
// ENUM: tipi di token riconosciuti dal lexer
// ==============================
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}							t_token_type;

typedef struct s_pipeline	t_pipeline;

typedef struct s_token
{
	char					**tokens;
	int						num_tokens;
	t_token_type			*token_types;

}							t_token;

// ==============================
// STRUTTURA di comando singolo
// ==============================
typedef struct s_cmd
{
	char					**args;
	char					*infile;
	char					*outfile;
	t_pipeline				*pipeline;
	int						append;
	int						heredoc;
	int						redir_in;
	int						redir_out;
	int						pipe;
	int						quote_single;
	int						quote_double;
	int						dollar;
	int						fd_in;
	int						fd_out;
}							t_cmd;

// ==============================
// STRUTTURA della PIPELINE
// ==============================
typedef struct s_pipeline
{
	char					**my_env;
	t_cmd					**cmds;
	int						n_cmds;
	char					**tokens;
	t_token_type			*types;
	int						n_tokens;
}							t_pipeline;

// ==============================
// LEXER
// Ritorna: numero token, oppure -1 in caso di errore (quote non chiuse ecc.)
// ==============================
int							lexer(char *line, t_token *tokens_struct);
// ==============================
// LEXER UTILS
// ==============================
int							is_metachar(char c);
int							ft_isspace(char c);
int							count_token(const char *line);
int							handle_redirection(const char *line, int i,
								t_token *tokens_struct, int *count);
int							fill_tokens(char *line, t_token *tokens_struct);
int							handle_word(const char *line, int i,
								t_token *tokens_struct, int *count);
void						exit_shell(int code, const char *msg);
void						free_matrix(char **data);
int							check_syntax(char **tokens, t_token_type *types,
								int ntokens);
int							skip_spaces(const char *line, int i);
int							check_backslash_error(const char *line, int i);
void						free_tokens_struct(t_token *tokens_struct);
void						clear_tokens_content(t_token *tokens_struct);

// ==============================
// BUILD PIPELINE
// ==============================
t_pipeline					*build_pipeline(char **tokens, t_token_type *types,
								int num_tokens, char **env);
void						free_pipeline(t_pipeline *pipeline);
void						populate_comands(t_pipeline *pipeline);
char						*strip_outer_quotes(const char *str);
int							count_args_for_cmds(t_token_type *types, int start,
								int n_tokens);
void						handle_pipe(t_pipeline *pipeline, t_cmd *cmd,
								int *i);
int							setup_redirections(t_cmd *cmd);
int							fill_cmds(t_pipeline *pipeline, t_cmd *cmd, int *i);
void						handle_command_redirection(t_pipeline *p,
								t_cmd *cmd, int *i);
t_cmd						*allocate_cmd(t_pipeline *pipeline, int n_args);

// ==============================
// PARSER
// ==============================
t_pipeline					*parse_line(char *line, char **env,
								t_pipeline *pipeline);

// ==============================
// DEBUG
// ==============================
// void    print_tokens(char **tokens, t_token_type *types);
// void    print_pipeline(t_pipeline *pipeline);
// ==============================
// GESTIONE QUOTES
// ==============================
char						*remove_quotes(const char *str);
void						expand_single_quotes(t_cmd *cmd);
void						find_quotes(t_pipeline *pipeline);
void						expand_quotes(t_pipeline *pipeline);
// ==============================
// Gestione backshalsh
// ==============================
// char    *handle_backslash(char *str);
char						*handle_backslash_outside_quotes(const char *str);

// ==============================
// ESPANSIONE VARIABILI
// ==============================
char						*expand_variables(const char *str, char **env);
void						expand_cmd_variables(t_cmd *cmd, char **env);
void						expand_pipeline_variables(t_pipeline *pipeline);
void						check_var(t_pipeline *pipeline);
char						*get_env_value(const char *var_name, char **env);
int							ft_strcmp(const char *s1, const char *s2);
int							get_env_value_len(const char *var_name, char **env);
int							compute_expanded_length(const char *str,
								char **env);
char						*get_var_name(const char *str, int *var_len);

#endif
