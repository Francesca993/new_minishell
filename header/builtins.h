/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:22:20 by francesca         #+#    #+#             */
/*   Updated: 2025/07/22 15:24:28 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		execute_builtin(t_cmd *cmd, char ***envp, t_pipeline *pipeline);
int		ft_exit(char **args);
void	init_signals(void);
int		ft_pwd(void);
int		ft_cd(char **args, char **my_env);
void	update_env_value(char **my_env, char *new_path);
int		unset_variable(char **my_env, char **args);
int		export_variable(char ***my_env, char **args);
void	print_export_buildin_sorted(char **my_env);
int		ft_echo(char **args);
int		is_builtin(t_cmd *cmd);
void	handle_sigquit(int signo);

#endif
