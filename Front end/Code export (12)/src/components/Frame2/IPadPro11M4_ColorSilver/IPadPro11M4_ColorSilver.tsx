import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import classes from './IPadPro11M4_ColorSilver.module.css';

interface Props {
  className?: string;
  classes?: {
    iPadPro11M4SilverLandscape?: string;
  };
}
/* @figmaId 153:62 */
export const IPadPro11M4_ColorSilver: FC<Props> = memo(function IPadPro11M4_ColorSilver(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <div className={`${props.classes?.iPadPro11M4SilverLandscape || ''} ${classes.iPadPro11M4SilverLandscape}`}></div>
    </div>
  );
});
