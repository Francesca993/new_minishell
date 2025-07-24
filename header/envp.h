/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:02:00 by francesca         #+#    #+#             */
/*   Updated: 2025/07/24 22:21:06 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

char	**copy_env(char **envp);
void	free_myenvp(char **my_env);
int		export_myenvp(char **my_env);
void	increment_shlvl(char ***envp);

#endif
