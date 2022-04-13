/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_bous.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeson <jeson@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 17:19:04 by taejkim           #+#    #+#             */
/*   Updated: 2022/04/11 17:43:01 by jeson            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

static int	is_dir_flag(char c)
{
	if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		return (1);
	return (0);
}

static void	scan_map(t_file *file)
{
	int	i;
	int	j;

	i = 6;
	if (!file->split[i])
		error_out("Error\ninvalid map");
	while (file->split[i])
	{
		j = 0;
		while (file->split[i][j])
			++j;
		if (file->map_w < j)
			file->map_w = j;
		++file->map_h;
		++i;
	}
}

static void	append_map(t_file *file)
{
	int	i;
	int	j;

	file->map = (char **)malloc(sizeof(char *) * (file->map_h + 1));
	i = 0;
	while (i < file->map_h)
	{
		file->map[i] = (char *)ft_calloc(file->map_w + 1, sizeof(char));
		j = 0;
		while (file->split[i + 6][j])
		{
			file->map[i][j] = file->split[i + 6][j];
			j++;
		}
		while (j < file->map_w)
			file->map[i][j++] = ' ';
		file->map[i][j] = 0;
		++i;
	}
	file->map[i] = 0;
}

static void	check_surround(t_file *file, int x, int y)
{
	if ((x == 0 || x == file->map_w - 1 || y == 0 || y == file->map_h - 1) \
		&& !(file->map[y][x] == '1' || file->map[y][x] == ' '))
			error_out("Error\ninvalid map");
	if (file->map[y][x] == ' ')
	{
		if (x != 0 && !(file->map[y][x - 1] == '1' \
					|| file->map[y][x - 1] == ' '))
			error_out("Error\ninvalid map");
		if (y != 0 && !(file->map[y - 1][x] == '1' \
					|| file->map[y - 1][x] == ' '))
			error_out("Error\ninvalid map");
		if (x != file->map_w - 1 && !(file->map[y][x + 1] == '1' \
					|| file->map[y][x + 1] == ' '))
			error_out("Error\ninvalid map");
		if (y != file->map_h - 1 && !(file->map[y + 1][x] == '1' \
					|| file->map[y + 1][x] == ' '))
			error_out("Error\ninvalid map");
	}
}

void	check_map(t_file *file)
{
	int x;
	int y;

	scan_map(file);
	append_map(file);
	y = -1;
	while (file->map[++y])
	{
		x = -1;
		while (file->map[y][++x])
		{
			if (!(file->map[y][x] == '1' || file->map[y][x] == '0' || file->map[y][x] == ' '))
			{
				if (!file->c_flag && is_dir_flag(file->map[y][x]))
				{
					file->c_x = x;
					file->c_y = y;
					file->c_flag = file->map[y][x];
				}
				else
					error_out("Error\ninvalid map");
			}
			check_surround(file, x , y);
		}
	}
}