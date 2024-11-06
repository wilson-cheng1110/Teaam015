import { memo, SVGProps } from 'react';

const Ellipse18Icon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 600 600' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path
      d='M300 32.9937C300 14.7718 314.816 -0.187549 332.928 1.81247C354.727 4.21972 376.198 9.01044 396.953 16.0984C414.197 21.9872 421.248 41.8257 413.502 58.3193V58.3193C405.756 74.8128 386.143 81.6576 368.724 76.3062C357.006 72.7061 345.022 70.0322 332.885 68.3095C314.844 65.7488 300 51.2156 300 32.9937V32.9937Z'
      fill='#009703'
    />
  </svg>
);

const Memo = memo(Ellipse18Icon);
export { Memo as Ellipse18Icon };
