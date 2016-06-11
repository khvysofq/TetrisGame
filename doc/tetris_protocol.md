# Tetris Protocol

- `game_state`
- `view_data`
- `next_tetrismino`
- `score`
- `token_id`
- `play_name`
- `status_code`

## Requestion

- `oper`
- `token_id`


### 开始游戏

每一次游戏都有一个名字，在开始游戏的时候`token_id`是一个输入项目，输入的是一个游戏名称，用户未来可以在服务器上查看到游戏的记录。`oper`只有几个操作，而且全部都是字符，全部都是小写字符

- `start`
- `stop`
- `pause`
- `resume`
- `up`
- `down`
- `left`
- `right`
- `get_state`