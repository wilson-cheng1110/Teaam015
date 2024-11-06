import { memo } from 'react';
import type { FC, ReactNode } from 'react';

import resets from '../../_resets.module.css';
import classes from './Search.module.css';
import { SearchIcon } from './SearchIcon.js';

interface Props {
  className?: string;
  swap?: {
    icon?: ReactNode;
  };
}
/* @figmaId 158:669 */
export const Search: FC<Props> = memo(function Search(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <div className={classes.icon}>{props.swap?.icon || <SearchIcon className={classes.icon2} />}</div>
    </div>
  );
});
