import { memo } from 'react';
import type { FC, ReactNode } from 'react';

import resets from '../../_resets.module.css';
import classes from './Settings.module.css';
import { SettingsIcon } from './SettingsIcon.js';

interface Props {
  className?: string;
  swap?: {
    icon?: ReactNode;
  };
}
/* @figmaId 158:673 */
export const Settings: FC<Props> = memo(function Settings(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <div className={classes.icon}>{props.swap?.icon || <SettingsIcon className={classes.icon2} />}</div>
    </div>
  );
});
